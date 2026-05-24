#pragma once
#include "SimulationTypes.h"
#include "SimulationObject.h"
#include <vector>
#include <memory>

class Factory {
private:
    int currentTick;
    bool isRunning;
    Scenario currentScenario;
    
    int statsFinished;
    int statsBreaks;
    int statsLost;
    
    int rawTimer;
    int nextPid;
    
    std::vector<std::string> eventLogs;
    std::vector<std::shared_ptr<SimulationObject>> simObjects;
    
    void logEvent(const std::string& msg, bool isError = false);

public:
    Factory();
    void init();
    void applyCommand(SimulationCommand& cmd);
    void tick();
    
    FactorySnapshot getSnapshot() const;
};