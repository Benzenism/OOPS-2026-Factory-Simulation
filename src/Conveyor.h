#pragma once
#include "SimulationObject.h"
#include "SimulationTypes.h"
#include "Products.h"
#include <deque>
#include <string>
#include <memory>
#include <functional>

using EventLogger  = std::function<void(const std::string&, bool)>;
using LostCallback = std::function<void()>;

class Conveyor : public SimulationObject {
private:
    struct SlottedItem {
        std::shared_ptr<Product> product;
        int arrivalTick = -1;
    };

    std::string id;
    int         delay;
    int         capacity;
    int         outputCount;

    std::deque<SlottedItem> items;

    EventLogger  logger;
    LostCallback onLost;

public:
    Conveyor(std::string id, int delay, int capacity);

    void setCallbacks(EventLogger l, LostCallback lost);

    void pushItem(std::shared_ptr<Product> item) override;
    void update(int tick) override;
    std::string getInfo() const override;
    MachineSnapshot getSnapshot() const override;
    void applyScenario(Scenario s) override;

    bool hasReady(int tick) const;
    std::shared_ptr<Product> pollReady(int tick);

    int getItemCount() const;
};