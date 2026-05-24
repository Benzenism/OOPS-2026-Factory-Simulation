#pragma once
#include "SimulationTypes.h"
#include <string>

class FactoryUI {
private:
    SimulationCommand currentCmd;
    std::string selectedMachineId = "";

public:
    void render(const FactorySnapshot& snap);
    SimulationCommand& getCommand();
};