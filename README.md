# Factory Simulation

GIST OOPS Final Project (2026-1)

## Project Structure

- src/ : source code
- build/ : executable files
- UML-diagram.pdf : UML class diagram

## Description

This project simulates a factory production line using object-oriented design principles.

Features:
- Factory simulation
- Machine state management
- Event logging
- Multiple simulation scenarios
- Interactive UI

## UML Diagram

```mermaid
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

%% Core Simulation Classes
class SimulationObject {
<<abstract>>
+~SimulationObject()
+update(tick: int)* void
+getInfo()* string
+pushItem(item: shared_ptr~Product~)* void
+getSnapshot()* MachineSnapshot
+applyScenario(s: Scenario)* void
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
+Machine(id: string, type: string, processTime: int, breakdownProb: float)
+setInputConveyor(c: shared_ptr~Conveyor~) void
+setOutputConveyor(c: shared_ptr~Conveyor~) void
+setCallbacks(l: EventLogger, f: OutputCallback, lost: LostCallback) void
+pushItem(item: shared_ptr~Product~) void
+update(tick: int) void
+getInfo() string
+getSnapshot() MachineSnapshot
+applyScenario(s: Scenario) void
+forceBreak() void
+instantRepair() void
+getState() MachineState
}

class Conveyor_SlottedItem {
<<struct>>
+shared_ptr~Product~ product
+int arrivalTick
}

class Conveyor {
-string id
-int delay
-int capacity
-int outputCount
-deque~Conveyor_SlottedItem~ items
-EventLogger logger
-LostCallback onLost
+Conveyor(id: string, delay: int, capacity: int)
+setCallbacks(l: EventLogger, lost: LostCallback) void
+pushItem(item: shared_ptr~Product~) void
+update(tick: int) void
+getInfo() string
+getSnapshot() MachineSnapshot
+applyScenario(s: Scenario) void
+hasReady(tick: int) bool
+pollReady(tick: int) shared_ptr~Product~
+getItemCount() int
}

%% Derived Machines
class WaferCutter {
+WaferCutter()
}

class ChipAssembler {
+ChipAssembler()
+applyScenario(s: Scenario) void
}

class ModuleTester {
+ModuleTester()
}

%% Products
class Product {
<<abstract>>
-string id
+Product(id: string)
+~Product()
+getId() string
+getType()* string
}

class RawWafer {
+RawWafer(id: string)
+getType() string
}

class RAMModule {
+RAMModule(id: string)
+getType() string
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
-logEvent(msg: string, isError: bool) void
+Factory()
+init() void
+applyCommand(cmd: SimulationCommand&) void
+tick() void
+getSnapshot() FactorySnapshot
}

class UIPanel {
<<abstract>>
+~UIPanel()
+render(snap: const FactorySnapshot&, cmd: SimulationCommand&)* void
}

class EventLogPanel {
+render(snap: const FactorySnapshot&, cmd: SimulationCommand&) void
}

class FactoryUI {
-EventLogPanel logPanel
-SimulationCommand currentCmd
-string selectedMachineId
+FactoryUI()
+render(snap: const FactorySnapshot&) void
+getCommand() SimulationCommand&
}

%% Inheritance Relationships
SimulationObject <|-- Machine
SimulationObject <|-- Conveyor
Machine <|-- WaferCutter
Machine <|-- ChipAssembler
Machine <|-- ModuleTester

Product <|-- RawWafer
Product <|-- RAMModule

UIPanel <|-- EventLogPanel

%% Composition and Aggregation Relationships
Factory "1" *-- "*" SimulationObject : owns(simObjects)
FactoryUI "1" *-- "1" EventLogPanel : owns(logPanel)
FactoryUI "1" *-- "1" SimulationCommand : owns(currentCmd)

%% Directed Associations (Reference passing)
Machine "1" --> "0..1" Conveyor : refers(inputConveyor)
Machine "1" --> "0..1" Conveyor : refers(outputConveyor)
Machine "1" --> "0..1" Product : handles(currentItem)

Conveyor "1" *-- "*" Conveyor_SlottedItem : contains(items)
Conveyor_SlottedItem "1" --> "1" Product : carries(product)

%% Dependency Relationships
FactoryUI ..> FactorySnapshot : reads
Factory ..> SimulationCommand : consumes
Factory ..> FactorySnapshot : generates

```
## UI Simulation

### 1. Normal Scenario

The Normal Scenario demonstrates the standard production flow of the RAM manufacturing process. Products move through each stage of the production line without bottlenecks or machine failures, showing the intended behavior of the system.

![Normal Scenario](simulation1.gif)

---

### 2. Overflow Scenario

The Overflow Scenario demonstrates how the factory behaves when products are generated faster than they can be processed. Queues begin to fill, bottlenecks appear, and products may be lost due to capacity limitations.

![Overflow Scenario](./simulation2.gif)

---

