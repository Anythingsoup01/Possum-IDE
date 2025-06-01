#include "Ferret/Core/Application.h"
#include "Ferret/Core/Entrypoint.h"
#include "PossumLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Ferret
{
    Possum::PossumLayer* possumLayer = new Possum::PossumLayer;

    Application* CreateApplication(int argc, char** argv)
    {
        ApplicationSpecifications spec;
        spec.Title = "PossumIDE";
        spec.DefaultIniLayout = "DefaultLayout.ini";

        Application* app = new Application(spec);
        app->SetDockNodeFlags(ImGuiDockNodeFlags_NoTabBar);
        app->PushLayer(possumLayer);
        app->SetMenubarCallback([app]()
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New File", "Ctrl + N"))
                    possumLayer->NewFile();
                if (ImGui::MenuItem("Open File", "Ctrl + O"))
                    possumLayer->OpenFile();
                if (ImGui::MenuItem("Open Folder", "Ctrl + Shift + O"))
                    possumLayer->OpenFolder();
                if (ImGui::MenuItem("Save File", "Ctrl + S"))
                    possumLayer->SaveFile();
                ImGui::EndMenu();
            }
        });

        return app;
    }
}
