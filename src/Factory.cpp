#include "Factory.h"
#include "RamMachines.h"
#include "Conveyor.h"

Factory::Factory() {
    init();
}

void Factory::logEvent(const std::string& msg, bool isError) {
    std::string prefix = isError ? "[WARN] " : "[INFO] ";
    std::string logLine = "[" + std::to_string(currentTick) + "] " + prefix + msg;
    eventLogs.push_back(logLine);
    if (eventLogs.size() > 80) eventLogs.erase(eventLogs.begin());
}

void Factory::init() {
    currentTick = 0;
    isRunning = false;
    currentScenario = Scenario::Normal;
    statsFinished = statsBreaks = statsLost = 0;
    rawTimer = 0;
    nextPid = 1;
    
    eventLogs.clear();
    simObjects.clear();

    auto cutter = std::make_shared<WaferCutter>();
    auto assembler = std::make_shared<ChipAssembler>();
    auto tester = std::make_shared<ModuleTester>();

    auto logger = [this](const std::string& m, bool e) { logEvent(m, e); };
    auto onLost = [this]() { statsLost++; };
    auto onFinish = [this, logger](std::shared_ptr<Product> p) {
        statsFinished++;
        logger("Finished product: " + p->getId(), false);
    };

    auto conveyor1 = std::make_shared<Conveyor>("Conveyor A", 2);
    auto conveyor2 = std::make_shared<Conveyor>("Conveyor B", 2);

    cutter->setNextNode(conveyor1);
    conveyor1->setNextNode(assembler);

    assembler->setNextNode(conveyor2);
    conveyor2->setNextNode(tester);
    
    cutter->setCallbacks(logger, nullptr, onLost);
    assembler->setCallbacks(logger, nullptr, onLost);
    tester->setCallbacks(logger, onFinish, onLost);

    simObjects.push_back(cutter);
    simObjects.push_back(conveyor1);

    simObjects.push_back(assembler);
    simObjects.push_back(conveyor2);

    simObjects.push_back(tester);

    for (auto& obj : simObjects) {
        if (auto m = std::dynamic_pointer_cast<Machine>(obj)) {
            m->applyScenario(currentScenario);
        }
    }
}

void Factory::applyCommand(SimulationCommand& cmd) {
    if (cmd.startWork) isRunning = true;
    if (cmd.pauseWork) isRunning = false;
    if (cmd.resetWork) init();
    if (cmd.clearLog) eventLogs.clear();

    if (cmd.selectedScenario != currentScenario) {
        currentScenario = cmd.selectedScenario;
        logEvent("Scenario changed.", false);
        for (auto& obj : simObjects) {
            if (auto m = std::dynamic_pointer_cast<Machine>(obj)) {
                m->applyScenario(currentScenario);
            }
        }
    }

    if (cmd.forceBreak && !cmd.targetMachineId.empty()) {
        for (auto& obj : simObjects) {
            if (auto m = std::dynamic_pointer_cast<Machine>(obj)) {
                if (m->getSnapshot().id == cmd.targetMachineId) {
                    m->forceBreak();
                    statsBreaks++;
                }
            }
        }
    }

    if (cmd.instantRepair && !cmd.targetMachineId.empty()) {
        for (auto& obj : simObjects) {
            if (auto m = std::dynamic_pointer_cast<Machine>(obj)) {
                if (m->getSnapshot().id == cmd.targetMachineId) {
                    m->instantRepair();
                }
            }
        }
    }
}

void Factory::tick() {
    if (!isRunning) return;
    currentTick++;

    rawTimer++;
    int injectInterval = 6; 
    if (currentScenario == Scenario::Overflow) {
        injectInterval = 2;
    }

    if (rawTimer >= injectInterval) {
        rawTimer = 0;
        std::string pid = "RAM-" + std::to_string(nextPid++);
        simObjects[0]->pushItem(std::make_shared<RawWafer>(pid));
    }

    for (auto& obj : simObjects) {
        obj->update(currentTick);
    }
}

FactorySnapshot Factory::getSnapshot() const {
    FactorySnapshot snap;
    snap.currentTick = currentTick;
    snap.finishedGoods = statsFinished;
    snap.totalBreakdowns = statsBreaks;
    snap.lostProducts = statsLost;
    snap.eventLogs = eventLogs;
    snap.wipCount = 0;

    for (const auto& obj : simObjects) {
        if (auto m = std::dynamic_pointer_cast<Machine>(obj)) {
            snap.machines.push_back(m->getSnapshot());
            snap.wipCount += m->getQueueSize();
            if (m->getState() == MachineState::WORKING) snap.wipCount++;
        }
    }
    return snap;
}
