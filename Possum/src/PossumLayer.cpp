#include "PossumLayer.h"
#include "Ferret.h"
#include "imgui.h"

namespace Ferret::Possum
{
    PossumLayer* PossumLayer::s_Instance = nullptr;

    void PossumLayer::OnAttach()
    {
        s_Instance = this;
    }
    
    void PossumLayer::OnDetach()
    {
        s_Instance = nullptr;
    }

    void PossumLayer::OnUpdate(float ts)
    {
        
    }

    void PossumLayer::OnUIRender()
    {
        ImGui::Begin("##WORKSPACE");
        {
            
            ImGui::End();
        }
    }

    void PossumLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        // Dispatch events
    }
}
