#pragma once
#include <memory>
#include <string>
#include "SimulationTypes.h"

class Product;

class SimulationObject {
public:
    virtual ~SimulationObject() = default;

    virtual void update(int tick) = 0;
    virtual std::string getInfo() const = 0;
    virtual void pushItem(std::shared_ptr<Product> item) = 0;
    virtual MachineSnapshot getSnapshot() const = 0;
    virtual void applyScenario(Scenario s) = 0;
};