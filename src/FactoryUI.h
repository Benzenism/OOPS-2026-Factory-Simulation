#pragma once
#include "SimulationTypes.h"
#include <string>
#include <EventLogPanel.h>

class FactoryUI {
private:
    EventLogPanel logPanel;
    SimulationCommand currentCmd;
    std::string selectedMachineId = "";

public:
    void render(const FactorySnapshot& snap);
    SimulationCommand& getCommand();
};
