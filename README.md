This is GIST OOPS Final Project in 2026-1 Semester.

UML:
'''mermaid
classDiagram
    %% Enumerations and Structs
    class MachineState {
        <<enumeration>>
        IDLE
        WORKING
        BROKEN
        MAINTENANCE
    }
    class Scenario {
        <<enumeration>>
        Normal
        Bottleneck
        RandomBreakdowns
        Overflow
    }
    class SimulationCommand {
        <<struct>>
        +bool startWork
        +bool pauseWork
        +bool resetWork
        +bool clearLog
        +bool forceBreak
        +bool instantRepair
        +string targetMachineId
        +Scenario selectedScenario
        +int speedMultiplier
        +clearCommandFlags() void
    }
    class MachineSnapshot {
        <<struct>>
        +string id
        +string type
        +MachineState state
        +float progress
        +int processTime
        +int health
        +int maxHealth
        +int queueSize
        +int capacity
        +int outputCount
        +bool isConveyor
        +int brokenTimer
    }
    class FactorySnapshot {
        <<struct>>
        +int currentTick
        +int finishedGoods
        +int wipCount
        +int totalBreakdowns
        +int lostProducts
        +vector~MachineSnapshot~ machines
        +vector~string~ eventLogs
    }
    class SlottedItem {
        <<struct>>
        +shared_ptr~Product~ product
        +int arrivalTick
    }
    %% Core Simulation Classes
    class SimulationObject {
        <<abstract>>
        +update(int tick)* void
        +getInfo() const* string
        +pushItem(shared_ptr~Product~ item)* void
        +getSnapshot() const* MachineSnapshot
        +applyScenario(Scenario s)* void
    }
    class Machine {
        #string id
        #string type
        #MachineState state
        #float progress
        #int processTime
        #int defaultProcessTime
        #int health
        #int maxHealth
        #float breakdownProb
        #int brokenTickCount
        #int outputCount
        #shared_ptr~Product~ currentItem
        #shared_ptr~Conveyor~ inputConveyor
        #shared_ptr~Conveyor~ outputConveyor
        #EventLogger logger
        #OutputCallback onFinish
        #LostCallback onLost
        #rollBreakdown() bool
        +Machine(string id, string type, int processTime, float breakdownProb)
        +setInputConveyor(shared_ptr~Conveyor~ c) void
        +setOutputConveyor(shared_ptr~Conveyor~ c) void
        +setCallbacks(EventLogger l, OutputCallback f, LostCallback lost) void
        +pushItem(shared_ptr~Product~ item) void
        +update(int tick) void
        +getInfo() const string
        +getSnapshot() const MachineSnapshot
        +applyScenario(Scenario s) void
        +forceBreak() void
        +instantRepair() void
        +getState() const MachineState
    }
    class Conveyor {
        -string id
        -int delay
        -int capacity
        -int outputCount
        -deque~SlottedItem~ items
        -EventLogger logger
        -LostCallback onLost
        +Conveyor(string id, int delay, int capacity)
        +setCallbacks(EventLogger l, LostCallback lost) void
        +pushItem(shared_ptr~Product~ item) void
        +update(int tick) void
        +getInfo() const string
        +getSnapshot() const MachineSnapshot
        +applyScenario(Scenario s) void
        +hasReady(int tick) const bool
        +pollReady(int tick) shared_ptr~Product~
        +getItemCount() const int
    }
    %% Derived Machines
    class WaferCutter {
        +WaferCutter()
    }
    class ChipAssembler {
        +ChipAssembler()
        +applyScenario(Scenario s) void
    }
    class ModuleTester {
        +ModuleTester()
    }
    %% Products
    class Product {
        <<abstract>>
        -string id
        +Product(string id)
        +getId() const string
        +getType() const* string
    }
    class RawWafer {
        +RawWafer(string id)
        +getType() const string
    }
    class RAMModule {
        +RAMModule(string id)
        +getType() const string
    }
    %% System Controllers and UI
    class Factory {
        -int currentTick
        -bool isRunning
        -Scenario currentScenario
        -int statsFinished
        -int statsBreaks
        -int statsLost
        -int rawTimer
        -int nextPid
        -vector~string~ eventLogs
        -vector~shared_ptr~SimulationObject~~ simObjects
        -logEvent(string msg, bool isError) void
        +Factory()
        +init() void
        +applyCommand(SimulationCommand& cmd) void
        +tick() void
        +getSnapshot() const FactorySnapshot
    }
    class UIPanel {
        <<abstract>>
        +render(const FactorySnapshot& snap, SimulationCommand& cmd)* void
    }
    class EventLogPanel {
        +render(const FactorySnapshot& snap, SimulationCommand& cmd) void
    }
    class FactoryUI {
        -EventLogPanel logPanel
        -SimulationCommand currentCmd
        -string selectedMachineId
        +FactoryUI()
        +render(const FactorySnapshot& snap) void
        +getCommand() SimulationCommand&
    }
    %% Relationships
    SimulationObject <|-- Machine
    SimulationObject <|-- Conveyor
    Machine <|-- WaferCutter
    Machine <|-- ChipAssembler
    Machine <|-- ModuleTester
    Product <|-- RawWafer
    Product <|-- RAMModule
    UIPanel <|-- EventLogPanel
    Factory *-- SimulationObject : contains
    FactoryUI *-- EventLogPanel : owns
    Machine "0..1" o-- "1" Conveyor : inputConveyor
    Machine "0..1" o-- "1" Conveyor : outputConveyor
    Machine o-- Product : currentItem
    Conveyor *-- SlottedItem : items
    SlottedItem o-- Product : product
    FactoryUI ..> SimulationCommand : uses
    FactoryUI ..> FactorySnapshot : uses
    Factory ..> SimulationCommand : uses
    Factory ..> FactorySnapshot : creates
'''
