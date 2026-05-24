#include "RamMachines.h"

WaferCutter::WaferCutter() 
    : Machine("Wafer Cutter", "Wafer Cutter", 3, 0.0f, 6) {}

ChipAssembler::ChipAssembler() 
    : Machine("Wafer Assembler", "Wafer Assembler", 5, 0.0f, 6) {}

void ChipAssembler::applyScenario(Scenario s) {
    Machine::applyScenario(s);
    
    if (s == Scenario::Bottleneck || s == Scenario::Overflow) {
        processTime = 12; 
    } else {
        processTime = defaultProcessTime;
    }
}

ModuleTester::ModuleTester() 
    : Machine("Module Tester", "Module Tester", 4, 0.0f, 6) {}