#include "Ferret/Core/Application.h"
#include "Ferret/Core/Entrypoint.h"
#include "PossumLayer.h"

#include "imgui.h"
#include "imgui_internal.h"

using namespace Ferret::Possum;
PossumLayer* possumLayer = new PossumLayer;

Ferret::Application* Ferret::CreateApplication(int argc, char** argv)
{
    Ferret::ApplicationSpecifications spec;
    spec.Title = "Possum IDE";

    Ferret::Application* app = new Ferret::Application(spec);
    app->SetDockspaceFlags(ImGuiDockNodeFlags_NoTabBar);
    app->PushLayer(possumLayer);
    app->SetMenubarCallback([app]()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New File", "Ctrl + N"))
            {
                possumLayer->NewFile();
            }
            if (ImGui::MenuItem("Open File", "Ctrl + O"))
            {
                possumLayer->Get().OpenFile();
            }
            if (ImGui::MenuItem("Save File", "Ctrl + S"))
            {
                possumLayer->Get().SaveFile();
            }

            if (ImGui::MenuItem("Exit"))
            {
                app->Close();
            }
            ImGui::EndMenu();
        }
    });
    return app;
}

