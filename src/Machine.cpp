#include "Machine.h"
#include "Conveyor.h"
#include <random>
#include <stdexcept>
#include <algorithm>

static constexpr int BROKEN_DELAY_TICKS = 5;

Machine::Machine(std::string id, std::string type,
                 int processTime, float breakdownProb)
    : id(std::move(id)), type(std::move(type)),
      processTime(processTime), defaultProcessTime(processTime),
      breakdownProb(breakdownProb)
{
    if (processTime <= 0)
        throw std::invalid_argument("Machine processTime must be > 0");

    state           = MachineState::IDLE;
    progress        = 0.0f;
    outputCount     = 0;
    maxHealth       = 100;
    health          = maxHealth;
    brokenTickCount = 0;
}

bool Machine::rollBreakdown() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen) < breakdownProb;
}

void Machine::setInputConveyor(std::shared_ptr<Conveyor> c) {
    inputConveyor = c;
}

void Machine::setOutputConveyor(std::shared_ptr<Conveyor> c) {
    outputConveyor = c;
}

void Machine::setCallbacks(EventLogger l, OutputCallback f, LostCallback lost) {
    logger   = l;
    onFinish = f;
    onLost   = lost;
}

void Machine::pushItem(std::shared_ptr<Product> item) {
    (void)item;
}

void Machine::update(int tick) {
    if (state == MachineState::BROKEN) {
        brokenTickCount++;
        if (brokenTickCount >= BROKEN_DELAY_TICKS) {
            brokenTickCount = 0;
            state = MachineState::MAINTENANCE;
            if (logger) logger(id + " technician dispatched (auto-repair started).", false);
        }
        return;
    }

    if (state == MachineState::MAINTENANCE) {
        health = std::min(maxHealth, health + 20);
        if (health >= maxHealth) {
            state = MachineState::IDLE;
            if (logger) logger(id + " repaired and back online.", false);
        }
        return;
    }

    if (state == MachineState::WORKING) {
        if (rollBreakdown()) {
            state           = MachineState::BROKEN;
            brokenTickCount = 0;
            health          = std::max(0, health - 25);
            if (logger) logger(id + " BROKEN DOWN! Waiting " + std::to_string(BROKEN_DELAY_TICKS) + " ticks for technician.", true);
            return;
        }

        health    = std::max(0, health - 1);
        progress += 1.0f;

        if (progress >= static_cast<float>(processTime)) {
            progress = 0.0f;
            outputCount++;

            if (outputConveyor && currentItem) {
                outputConveyor->pushItem(currentItem);
            } else if (onFinish && currentItem) {
                onFinish(currentItem);
            }

            currentItem = nullptr;
            state       = MachineState::IDLE;
        }
    }

    if (state == MachineState::IDLE) {
        if (health <= 0) {
            state           = MachineState::MAINTENANCE;
            brokenTickCount = 0;
            if (logger) logger(id + " auto-maintenance started.", false);
        } else if (inputConveyor && inputConveyor->hasReady(tick)) {
            currentItem = inputConveyor->pollReady(tick);
            if (currentItem) {
                state = MachineState::WORKING;
            }
        }
    }
}

std::string Machine::getInfo() const {
    return id + " (" + type + ")";
}

void Machine::applyScenario(Scenario s) {
    breakdownProb = (s == Scenario::RandomBreakdowns) ? 0.06f : 0.0f;
    processTime   = defaultProcessTime;
}

void Machine::forceBreak() {
    if (state != MachineState::BROKEN && state != MachineState::MAINTENANCE) {
        state           = MachineState::BROKEN;
        brokenTickCount = 0;
        health          = 0;
        if (logger) logger(id + " forced break by user.", true);
    }
}

void Machine::instantRepair() {
    state           = MachineState::IDLE;
    brokenTickCount = 0;
    health          = maxHealth;
    if (logger) logger(id + " instantly repaired by user.", false);
}

MachineSnapshot Machine::getSnapshot() const {
    MachineSnapshot snap;
    snap.id          = id;
    snap.type        = type;
    snap.state       = state;
    snap.progress    = progress;
    snap.processTime = processTime;
    snap.health      = health;
    snap.maxHealth   = maxHealth;
    snap.queueSize   = inputConveyor ? inputConveyor->getItemCount() : 0;
    snap.capacity    = 0;
    snap.outputCount = outputCount;
    snap.isConveyor  = false;
    snap.brokenTimer = brokenTickCount;
    return snap;
}

MachineState Machine::getState() const {
    return state;
}