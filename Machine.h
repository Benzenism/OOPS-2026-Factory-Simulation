#pragma once
#include "SimulationObject.h"
#include "SimulationTypes.h"
#include "Products.h"
#include <queue>
#include <functional>

using EventLogger = std::function<void(const std::string&, bool)>;
using OutputCallback = std::function<void(std::shared_ptr<Product>)>;
using LostCallback = std::function<void()>;

class Machine : public SimulationObject {
protected:
    std::string id;
    std::string type;
    
    MachineState state;
    float progress;
    int processTime;
    int defaultProcessTime;
    
    int health;
    int maxHealth;
    float breakdownProb; 
    
    int queueCapacity;
    int outputCount;
    
    std::queue<std::shared_ptr<Product>> inputQueue;
    std::shared_ptr<Product> currentItem;
    std::shared_ptr<SimulationObject> nextNode;

    EventLogger logger;
    OutputCallback onFinish;
    LostCallback onLost;

    bool rollBreakdown();

public:
    Machine(std::string id, std::string type, int processTime, float breakdownProb, int capacity);
    
    void setNextNode(std::shared_ptr<SimulationObject> next);
    void setCallbacks(EventLogger l, OutputCallback f, LostCallback lost);

    void pushItem(std::shared_ptr<Product> item) override;
    void update(int tick) override;
    std::string getInfo() const override;

    virtual void applyScenario(Scenario s);
    
    void forceBreak();
    void instantRepair();
    
    MachineSnapshot getSnapshot() const;
    int getQueueSize() const;
    MachineState getState() const;
};