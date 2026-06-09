#pragma once
#include "UIPanel.h"

class EventLogPanel : public UIPanel {
public:
    void render(const FactorySnapshot& snap, SimulationCommand& cmd) override;
};