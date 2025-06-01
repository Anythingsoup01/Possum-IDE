#pragma once
#include "Ferret.h"
#include "Ferret/Event/KeyEvent.h"

#include "Core/FileManager.h"
#include "Core/ProjectManager.h"
#include "Core/ImGuiRenderer.h"

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
 
        static PossumLayer* Get() { return s_Instance; }

        void NewFile();
        void OpenFile();
        void OpenFolder();
        void SaveFile();

        void SetCurrentFileData(const std::string& data, const std::filesystem::path& filePath) { m_CurrentFileString = data; m_CurrentFilePath = filePath; }
        ProjectManager& GetProjectManager() { return m_ProjectManager; }

        bool IsUsingProjectData() { return m_ProjectManager.InUse(); }

        const std::string& GetCurrentFileString() { return m_CurrentFileString; }
        const std::filesystem::path& GetCurrentFilePath() { return m_CurrentFilePath; }
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
    private:
        static inline PossumLayer* s_Instance = nullptr;

        ImGuiRenderer m_ImGuiRenderer;
        ProjectManager m_ProjectManager;
        FileManager m_FileManager;

        std::string m_CurrentFileString;
        std::filesystem::path m_CurrentFilePath;
    };
}
