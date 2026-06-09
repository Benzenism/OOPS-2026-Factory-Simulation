#pragma once
#include "SimulationObject.h"
#include "SimulationTypes.h"
#include "Products.h"
#include <functional>
#include <string>
#include <memory>

using EventLogger    = std::function<void(const std::string&, bool)>;
using OutputCallback = std::function<void(std::shared_ptr<Product>)>;
using LostCallback   = std::function<void()>;

class Conveyor;

class Machine : public SimulationObject {
protected:
    std::string id;
    std::string type;

    MachineState state;
    float        progress;
    int          processTime;
    int          defaultProcessTime;

    int   health;
    int   maxHealth;
    float breakdownProb;
    int   brokenTickCount;

    int outputCount;

    std::shared_ptr<Product>          currentItem;
    std::shared_ptr<Conveyor>         inputConveyor;
    std::shared_ptr<Conveyor>         outputConveyor;

    EventLogger    logger;
    OutputCallback onFinish;
    LostCallback   onLost;

    bool rollBreakdown();

public:
    Machine(std::string id, std::string type,
            int processTime, float breakdownProb);

    void setInputConveyor(std::shared_ptr<Conveyor> c);
    void setOutputConveyor(std::shared_ptr<Conveyor> c);
    void setCallbacks(EventLogger l, OutputCallback f, LostCallback lost);

    void pushItem(std::shared_ptr<Product> item) override;
    void update(int tick) override;
    std::string getInfo() const override;
    MachineSnapshot getSnapshot() const override;
    virtual void applyScenario(Scenario s) override;

    void forceBreak();
    void instantRepair();

    MachineState getState() const;
};