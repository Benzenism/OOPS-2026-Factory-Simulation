#pragma once
#include <string>
#include <vector>

enum class Scenario {
    Normal,
    Bottleneck,
    RandomBreakdowns,
    Overflow
};

enum class MachineState {
    IDLE,
    WORKING,
    BROKEN,
    MAINTENANCE
};

struct SimulationCommand {
    bool startWork     = false;
    bool pauseWork     = false;
    bool resetWork     = false;
    bool clearLog      = false;
    bool forceBreak    = false;
    bool instantRepair = false;
    std::string targetMachineId = "";
    Scenario selectedScenario   = Scenario::Normal;
    int speedMultiplier         = 2;

    void clearCommandFlags() {
        startWork = pauseWork = resetWork = clearLog = false;
        forceBreak = instantRepair = false;
        targetMachineId = "";
    }
};

struct MachineSnapshot {
    std::string  id;
    std::string  type;
    MachineState state       = MachineState::IDLE;
    float        progress    = 0.0f;
    int          processTime = 0;
    int          health      = 0;
    int          maxHealth   = 0;
    int          queueSize   = 0;
    int          capacity    = 0;
    int          outputCount = 0;
    bool         isConveyor  = false;
    int          brokenTimer = 0;
};

struct FactorySnapshot {
    int currentTick     = 0;
    int finishedGoods   = 0;
    int wipCount        = 0;
    int totalBreakdowns = 0;
    int lostProducts    = 0;
    std::vector<MachineSnapshot> machines;
    std::vector<std::string>     eventLogs;
};