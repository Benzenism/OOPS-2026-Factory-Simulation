#pragma once

#include "SimulationObject.h"
#include "SimulationTypes.h"
#include "Products.h"
#include <queue>
#include <memory>
#include <string>

class Conveyor : public SimulationObject {
private:
    std::string id;

    std::queue<std::shared_ptr<Product>> items;

    std::shared_ptr<SimulationObject> nextNode;

    int delay;
    int currentTick;

public:
    Conveyor(std::string id, int delay);

    void setNextNode(std::shared_ptr<SimulationObject> next);

    void pushItem(std::shared_ptr<Product> item) override;

    void update(int tick) override;

    std::string getInfo() const override;

    MachineSnapshot getSnapshot() const;
};
