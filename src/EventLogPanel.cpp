#include "EventLogPanel.h"
#include "imgui.h"

void EventLogPanel::render(const FactorySnapshot& snap, SimulationCommand& cmd) {
    ImGui::BeginChild("EventLogPanel", ImVec2(0, 0), true);

    ImGui::Text("EVENT LOG");
    ImGui::SameLine(ImGui::GetWindowWidth() - 70);
    if (ImGui::Button("Clear"))
        cmd.clearLog = true;

    ImGui::Separator();

    ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

    for (auto it = snap.eventLogs.rbegin(); it != snap.eventLogs.rend(); ++it) {
        ImVec4 color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
        if (it->find("[WARN]") != std::string::npos)
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
        else if (it->find("[INFO]") != std::string::npos)
            color = ImVec4(0.4f, 0.8f, 1.0f, 1.0f);
        ImGui::TextColored(color, "%s", it->c_str());
    }

    ImGui::EndChild();
    ImGui::EndChild();
}