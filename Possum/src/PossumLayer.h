#pragma once

#include "Ferret/Core/Application.h"

#include "Ferret/Layer/Layer.h"
#include "Ferret/Event/Event.h"
#include "Ferret/Event/KeyEvent.h"

#include "Core/FileManager.h"

#include "ImGuiFileDialog.h"

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

        void NewFile();
        void OpenFolder();
        void OpenFile();
        void SaveFile();


    private:
        
        bool OnKeyPressed(KeyPressedEvent& e);
        void RenderFileDialog();
        void RenderWorkspace();
        void RenderFileTree();

        void RenderOpenFile();
        void RenderOpenFolder();
        void RenderSaveFileAs();

       

    private:

        struct FileData
        {
            std::string FileName = "";
        };

        struct FolderData
        {
            std::string FolderName = "";
            std::map<std::filesystem::path, FileData> Files;
            std::map<std::filesystem::path, FolderData> SubFolders;
        };

        struct ProjectData
        {
            std::string ProjectName = "";
            std::filesystem::path ProjectPath = "";
            std::map<std::filesystem::path, FolderData> Folders;
            std::map<std::filesystem::path, FileData> Files;
        };
        
        void RenderFolderData(const std::map<std::filesystem::path, FolderData>& folderData);
        void RenderFileData(const std::map<std::filesystem::path, FileData>& fileData);
        void EmplaceFolderData(std::map<std::filesystem::path, FolderData>& folderData, const std::filesystem::path& folderPath);
        void EmplaceFileData(std::map<std::filesystem::path, FileData>& fileData, const std::filesystem::path& folderPath);
        // This is used when opening folders or opening projects
        ProjectData m_ProjectData;
        bool m_UsingProjectData = false;
    private:
        static PossumLayer* s_Instance;
        FileManager m_FileManager;

        std::string m_CurrentFileString;
        std::filesystem::path m_CurrentFilePath;
        std::string m_CurrentFileName;

        CBuffer m_FileNameBuffer;
        CBuffer m_FileDirectoryBuffer;

        ImGuiFileDialog m_OpenFileDialog;

    private:
        // Would like to limit the booleans for file open, save, ect.
        // to just one, then we can use states to control what it's
        // gonna do

        enum class FileInteractionType
        {
            None = 0,
            Open = 1,
            Folder = 2,
            Save = 3,
        };
        bool m_FileInteraction = false;
        FileInteractionType m_FileInteractionType = FileInteractionType::None;
    };
}
