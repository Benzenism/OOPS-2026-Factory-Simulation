#pragma once
#include "SimulationTypes.h"
#include "EventLogPanel.h"
#include <string>

class FactoryUI {
private:
    EventLogPanel    logPanel;
    SimulationCommand currentCmd;
    std::string       selectedMachineId;

public:
    FactoryUI();
    void render(const FactorySnapshot& snap);
    SimulationCommand& getCommand();
};