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
    bool startWork = false;
    bool pauseWork = false;
    bool resetWork = false;
    bool clearLog = false;
    
    bool forceBreak = false;
    bool instantRepair = false;
    std::string targetMachineId = "";

    Scenario selectedScenario = Scenario::Normal;
    int speedMultiplier = 2;

    void clearCommandFlags() {
        startWork = false;
        pauseWork = false;
        resetWork = false;
        clearLog = false;
        forceBreak = false;
        instantRepair = false;
        targetMachineId = "";
    }
};

struct MachineSnapshot {
    std::string id;
    std::string type;
    MachineState state;
    float progress;
    int processTime;
    int health;
    int maxHealth;
    int queueSize;
    int capacity;
    int outputCount;
};

struct FactorySnapshot {
    int currentTick = 0;
    int finishedGoods = 0;
    int wipCount = 0;
    int totalBreakdowns = 0;
    int lostProducts = 0;
    std::vector<MachineSnapshot> machines;
    std::vector<std::string> eventLogs;
};