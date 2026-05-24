#include "Machine.h"
#include <random>
#include <algorithm>

Machine::Machine(std::string id, std::string type, int processTime, float breakdownProb, int capacity)
    : id(std::move(id)), type(std::move(type)), processTime(processTime), defaultProcessTime(processTime),
      breakdownProb(breakdownProb), queueCapacity(capacity) {
    state = MachineState::IDLE;
    progress = 0.0f;
    outputCount = 0;
    maxHealth = 100;
    health = maxHealth;
}

bool Machine::rollBreakdown() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen) < breakdownProb;
}

void Machine::setNextNode(std::shared_ptr<SimulationObject> next) {
    nextNode = next;
}

void Machine::setCallbacks(EventLogger l, OutputCallback f, LostCallback lost) {
    logger = l;
    onFinish = f;
    onLost = lost;
}

void Machine::pushItem(std::shared_ptr<Product> item) {
    if (inputQueue.size() < queueCapacity) {
        inputQueue.push(item);
    } else {
        if (logger) logger("Overflow at " + id + ": Product " + item->getId() + " lost.", true);
        if (onLost) onLost();
    }
}

void Machine::update(int tick) {
    if (state == MachineState::MAINTENANCE) {
        health = std::min(maxHealth, health + 20);
        if (health >= maxHealth) {
            state = MachineState::IDLE;
            if (logger) logger(id + " repaired and back online.", false);
        }
        return;
    }

    if (state == MachineState::BROKEN) {
        state = MachineState::MAINTENANCE;
        if (logger) logger(id + " technician dispatched (auto-repair started).", false);
        return;
    }

    if (state == MachineState::WORKING) {
        if (rollBreakdown()) {
            state = MachineState::BROKEN;
            health = std::max(0, health - 25);
            if (logger) logger(id + " BROKEN DOWN!", true);
            return;
        }

        health = std::max(0, health - 1);
        progress += 1.0f;

        if (progress >= processTime) {
            progress = 0.0f;
            outputCount++;
            
            if (nextNode && currentItem) {
                nextNode->pushItem(currentItem);
            } else if (onFinish && currentItem) {
                onFinish(currentItem);
            }
            
            currentItem = nullptr;
            state = MachineState::IDLE;
        }
    }

    if (state == MachineState::IDLE) {
        if (health <= 0) {
            state = MachineState::MAINTENANCE;
            if (logger) logger(id + " auto-maintenance started.", false);
        } else if (!inputQueue.empty()) {
            currentItem = inputQueue.front();
            inputQueue.pop();
            state = MachineState::WORKING;
        }
    }
}

std::string Machine::getInfo() const {
    return id + " (" + type + ")";
}

void Machine::applyScenario(Scenario s) {
    if (s == Scenario::RandomBreakdowns) {
        breakdownProb = 0.06f; 
    } else {
        breakdownProb = 0.0f;
    }
    processTime = defaultProcessTime;
}

void Machine::forceBreak() {
    if (state != MachineState::BROKEN && state != MachineState::MAINTENANCE) {
        state = MachineState::BROKEN;
        health = 0;
        if (logger) logger(id + " forced break by user.", true);
    }
}

void Machine::instantRepair() {
    state = MachineState::IDLE;
    health = maxHealth;
    if (logger) logger(id + " instantly repaired by user.", false);
}

MachineSnapshot Machine::getSnapshot() const {
    MachineSnapshot snap;
    snap.id = id;
    snap.type = type;
    snap.state = state;
    snap.progress = progress;
    snap.processTime = processTime;
    snap.health = health;
    snap.maxHealth = maxHealth;
    snap.queueSize = (int)inputQueue.size();
    snap.capacity = queueCapacity;
    snap.outputCount = outputCount;
    return snap;
}

int Machine::getQueueSize() const {
    return (int)inputQueue.size();
}

MachineState Machine::getState() const {
    return state;
}
