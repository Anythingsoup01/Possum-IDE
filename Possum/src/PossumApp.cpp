#include "Ferret/Core/Application.h"
#include "Ferret/Core/Entrypoint.h"
#include "PossumLayer.h"
#include "imgui.h"

using namespace Ferret::Possum;
PossumLayer* possumLayer = new PossumLayer;

Ferret::Application* Ferret::CreateApplication(int argc, char** argv)
{
    Ferret::ApplicationSpecifications spec;
    spec.Title = "Possum IDE";

    Ferret::Application* app = new Ferret::Application(spec);
    app->PushLayer(possumLayer);
    app->SetMenubarCallback([app]()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                app->Close();
            }
            ImGui::EndMenu();
        }
    });
    return app;
}

