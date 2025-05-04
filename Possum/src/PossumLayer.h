#pragma once

#include "Ferret/Core/Application.h"

#include "Ferret/Layer/Layer.h"
#include "Ferret/Event/Event.h"
#include "Ferret/Event/KeyEvent.h"

#include "Core/FileManager.h"

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

        void OpenFile();
        void SaveFile();

    private:
        
        bool OnKeyPressed(KeyPressedEvent& e);
        void RenderFileDialog();
        void RenderWorkspace();
        void RenderFileTree();

        void RenderOpenFile();
        void RenderSaveFileAs();

    private:
        static PossumLayer* s_Instance;
        FileManager m_FileManager;

        std::string m_CurrentFileString;
        std::filesystem::path m_CurrentFilePath;

        char* m_FileNameBuffer;

    private:
        // Would like to limit the booleans for file open, save, ect.
        // to just one, then we can use states to control what it's
        // gonna do

        enum class FileInteractionType
        {
            None = 0,
            Open = 1,
            Save = 2,
        };
        bool m_FileInteraction = false;
        FileInteractionType m_FileInteractionType = FileInteractionType::None;
    };
}
