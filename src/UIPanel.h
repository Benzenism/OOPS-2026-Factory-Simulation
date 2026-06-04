#pragma once

#include "SimulationTypes.h"

class UIPanel {
public:
    virtual ~UIPanel() = default;

    virtual void render(
        const FactorySnapshot& snap,
        SimulationCommand& cmd
    ) = 0;
};
