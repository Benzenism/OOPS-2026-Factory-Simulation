#include "Conveyor.h"

Conveyor::Conveyor(std::string id, int delay)
    : id(std::move(id)), delay(delay), currentTick(0) {}

void Conveyor::setNextNode(std::shared_ptr<SimulationObject> next) {
    nextNode = next;
}

void Conveyor::pushItem(std::shared_ptr<Product> item) {
    items.push(item);
}

void Conveyor::update(int tick) {
    currentTick++;

    if (!items.empty() && currentTick >= delay) {
        if (nextNode) {
            nextNode->pushItem(items.front());
        }

        items.pop();
        currentTick = 0;
    }
}

std::string Conveyor::getInfo() const {
    return id;
}

MachineSnapshot Conveyor::getSnapshot() const {
    MachineSnapshot snap;

    snap.id = id;
    snap.type = "Conveyor";

    snap.state = MachineState::WORKING;

    snap.progress = (float)items.size();
    snap.processTime = delay;

    snap.health = 100;
    snap.maxHealth = 100;

    snap.queueSize = (int)items.size();
    snap.capacity = 999;

    snap.outputCount = 0;

    return snap;
}
