#include "Conveyor.h"
#include <stdexcept>

Conveyor::Conveyor(std::string id, int delay, int capacity)
    : id(std::move(id)), delay(delay), capacity(capacity), outputCount(0)
{
    if (delay <= 0)
        throw std::invalid_argument("Conveyor delay must be > 0");
    if (capacity <= 0)
        throw std::invalid_argument("Conveyor capacity must be > 0");
}

void Conveyor::setCallbacks(EventLogger l, LostCallback lost) {
    logger = l;
    onLost = lost;
}

void Conveyor::pushItem(std::shared_ptr<Product> item) {
    if (!item) return;
    if (static_cast<int>(items.size()) >= capacity) {
        if (logger) logger("Conveyor " + id + " overflow: Product " + item->getId() + " lost.", true);
        if (onLost)  onLost();
        return;
    }
    items.push_back({item, -1});
}

void Conveyor::update(int tick) {
    for (auto& slot : items) {
        if (slot.arrivalTick < 0) {
            slot.arrivalTick = tick;
        }
    }
}

bool Conveyor::hasReady(int tick) const {
    if (items.empty()) return false;
    const auto& front = items.front();
    return front.arrivalTick >= 0 && (tick - front.arrivalTick) >= delay;
}

std::shared_ptr<Product> Conveyor::pollReady(int tick) {
    if (!hasReady(tick)) return nullptr;
    auto product = items.front().product;
    items.pop_front();
    outputCount++;
    return product;
}

std::string Conveyor::getInfo() const {
    return id + " [Conveyor] items=" + std::to_string(items.size())
           + "/" + std::to_string(capacity);
}

MachineSnapshot Conveyor::getSnapshot() const {
    MachineSnapshot snap;
    snap.id          = id;
    snap.type        = "Conveyor";
    snap.state       = items.empty() ? MachineState::IDLE : MachineState::WORKING;
    snap.progress    = static_cast<float>(items.size());
    snap.processTime = delay;
    snap.health      = 100;
    snap.maxHealth   = 100;
    snap.queueSize   = static_cast<int>(items.size());
    snap.capacity    = capacity;
    snap.outputCount = outputCount;
    snap.isConveyor  = true;
    return snap;
}

void Conveyor::applyScenario(Scenario s) {
    (void)s;
}

int Conveyor::getItemCount() const {
    return static_cast<int>(items.size());
}