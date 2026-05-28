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
