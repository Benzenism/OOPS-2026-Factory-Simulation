#include "FactoryUI.h"
#include "imgui.h"
#include <cstdio>

void FactoryUI::render(const FactorySnapshot& snap) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1260, 700), ImGuiCond_FirstUseEver);

    ImGui::Begin("RAM Factory Simulation Dashboard", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::BeginChild("ControlPanel", ImVec2(0, 50), true);
    if (ImGui::Button("Start")) currentCmd.startWork = true;
    ImGui::SameLine();
    if (ImGui::Button("Pause")) currentCmd.pauseWork = true;
    ImGui::SameLine();
    if (ImGui::Button("Reset")) currentCmd.resetWork = true;
    
    ImGui::SameLine(0, 30.0f);
    ImGui::SetNextItemWidth(150.0f);
    ImGui::SliderInt("Speed", &currentCmd.speedMultiplier, 1, 5); 

    ImGui::SameLine();
    const char* scenarios[] = { "Normal Flow", "Bottleneck", "Random Breakdowns", "Overflow" }; 
    static int item_current = 0;
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::Combo("Scenario", &item_current, scenarios, IM_ARRAYSIZE(scenarios))) {
        currentCmd.selectedScenario = static_cast<Scenario>(item_current);
    }

    ImGui::SameLine(ImGui::GetWindowWidth() - 120.0f);
    ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.8f, 1.0f), "TICK : %04d", snap.currentTick);
    ImGui::EndChild();

    ImGui::BeginChild("StatsPanel", ImVec2(0, 60), true);
    ImGui::Columns(4, "stat_columns", false);
    
    ImGui::TextDisabled("FINISHED GOODS");
    ImGui::TextColored(ImVec4(0.2f,0.8f,0.2f,1.0f), "%d", snap.finishedGoods);
    ImGui::NextColumn();
    
    ImGui::TextDisabled("IN PROGRESS (WIP)");
    ImGui::TextColored(ImVec4(0.4f,0.6f,1.0f,1.0f), "%d", snap.wipCount);
    ImGui::NextColumn();
    
    ImGui::TextDisabled("BREAKDOWNS");
    ImGui::TextColored(ImVec4(0.9f,0.6f,0.1f,1.0f), "%d", snap.totalBreakdowns);
    ImGui::NextColumn();
    
    ImGui::TextDisabled("LOST PRODUCTS");
    ImGui::TextColored(ImVec4(0.9f,0.2f,0.2f,1.0f), "%d", snap.lostProducts);
    
    ImGui::Columns(1);
    ImGui::EndChild();

    ImGui::Columns(2, "MainLayout", false);
    ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.7f);

    ImGui::BeginChild("FloorPanel", ImVec2(0, 300), true);
    ImGui::Text("FACTORY FLOOR"); ImGui::Separator();
    
    for (const auto& m : snap.machines) {
        ImGui::PushID(m.id.c_str());
        
        bool isSelected = (selectedMachineId == m.id);
        ImVec2 startPos = ImGui::GetCursorPos();
        
        if (ImGui::Selectable((std::string("##row_") + m.id).c_str(), isSelected, ImGuiSelectableFlags_AllowOverlap, ImVec2(0, 25))) {
            selectedMachineId = m.id;
        }
        
        ImGui::SetCursorPos(ImVec2(startPos.x, startPos.y + 4)); 
        ImGui::Text("%s", m.id.c_str());
        
        ImVec4 stateColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        if (m.state == MachineState::WORKING) stateColor = ImVec4(0.2f, 0.9f, 0.2f, 1.0f);
        if (m.state == MachineState::BROKEN) stateColor = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
        if (m.state == MachineState::MAINTENANCE) stateColor = ImVec4(0.9f, 0.7f, 0.1f, 1.0f);

        ImGui::SameLine(160);
        ImGui::TextColored(stateColor, m.state == MachineState::WORKING ? "WORKING" :
                                       m.state == MachineState::BROKEN ? "BROKEN" :
                                       m.state == MachineState::MAINTENANCE ? "MAINT." : "IDLE");

        ImGui::SameLine(250);
        float progressRatio = m.processTime > 0 ? (float)m.progress / m.processTime : 0.0f;
        ImGui::SetNextItemWidth(250);
        ImGui::ProgressBar(progressRatio, ImVec2(0.0f, 0.0f), "Progress");

        ImGui::SameLine(520);
        ImGui::TextDisabled("Q: %d/%d | Out: %d", m.queueSize, m.capacity, m.outputCount);
        
        ImGui::SetCursorPosY(startPos.y + 28);
        ImGui::PopID();
        ImGui::Separator();
    }
    ImGui::EndChild();

    ImGui::BeginChild("InspectorPanel", ImVec2(0, 0), true);
    ImGui::Text("INSPECTOR"); ImGui::Separator();
    
    if (selectedMachineId.empty()) {
        ImGui::TextDisabled("Select a machine from the floor.");
    } else {
        for (const auto& m : snap.machines) {
            if (m.id == selectedMachineId) {
                ImGui::Text("ID: %s", m.id.c_str());
                ImGui::Text("Type: %s", m.type.c_str());
                ImGui::Spacing();
                
                float healthRatio = m.maxHealth > 0 ? (float)m.health / (float)m.maxHealth : 0.0f;
                char healthOverlay[32];
                snprintf(healthOverlay, sizeof(healthOverlay), "Health: %d / %d", m.health, m.maxHealth);
                
                float availWidth = ImGui::GetContentRegionAvail().x;
                ImVec2 barPos = ImGui::GetCursorPos();
                
                ImGui::SetNextItemWidth(-1);
                ImGui::ProgressBar(healthRatio, ImVec2(0.0f, 0.0f), ""); 
                
                ImVec2 nextPos = ImGui::GetCursorPos();
                
                float textWidth = ImGui::CalcTextSize(healthOverlay).x;
                ImGui::SetCursorPos(ImVec2(barPos.x + (availWidth - textWidth) * 0.5f, barPos.y + 2.0f));
                ImGui::Text("%s", healthOverlay);
                
                ImGui::SetCursorPos(nextPos);
                ImGui::Spacing();

                if (ImGui::Button("Force Break", ImVec2(120, 30))) { 
                    currentCmd.forceBreak = true;
                    currentCmd.targetMachineId = m.id;
                }
                ImGui::SameLine();
                if (ImGui::Button("Instant Repair", ImVec2(120, 30))) { 
                    currentCmd.instantRepair = true;
                    currentCmd.targetMachineId = m.id;
                }
            }
        }
    }
    ImGui::EndChild();

    ImGui::NextColumn();

    ImGui::BeginChild("EventLogPanel", ImVec2(0, 0), true);
    ImGui::Text("EVENT LOG");
    ImGui::SameLine(ImGui::GetWindowWidth() - 70);
    if (ImGui::Button("Clear")) currentCmd.clearLog = true;
    ImGui::Separator();
    
    ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto it = snap.eventLogs.rbegin(); it != snap.eventLogs.rend(); ++it) {
        ImVec4 color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
        if (it->find("[WARN]") != std::string::npos) color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
        else if (it->find("[INFO]") != std::string::npos) color = ImVec4(0.4f, 0.8f, 1.0f, 1.0f);
        ImGui::TextColored(color, "%s", it->c_str());
    }
    ImGui::EndChild();
    
    ImGui::EndChild();
    ImGui::End();
}

SimulationCommand& FactoryUI::getCommand() {
    return currentCmd;
}