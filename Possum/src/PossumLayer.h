#pragma once

#include "Ferret/Layer/Layer.h"
#include "Ferret/Event/Event.h"
#include "Ferret/Event/KeyEvent.h"

namespace Ferret::Possum
{
    class PossumLayer : public Layer
    {
    public:
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(float ts) override;
        virtual void OnUIRender() override;
        virtual void OnEvent(Event& event) override;

        static PossumLayer& Get() { return *s_Instance; }
        
    private:
        static PossumLayer* s_Instance;
    };
}
