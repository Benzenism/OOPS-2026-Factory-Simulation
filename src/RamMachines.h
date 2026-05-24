#pragma once
#include "Machine.h"

class WaferCutter : public Machine {
public:
    WaferCutter();
};

class ChipAssembler : public Machine {
public:
    ChipAssembler();
    void applyScenario(Scenario s) override;
};

class ModuleTester : public Machine {
public:
    ModuleTester();
};
