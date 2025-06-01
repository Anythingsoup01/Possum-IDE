#pragma once

#include "ProjectManager.h"
#include "Buffer.h"

namespace Ferret::Possum
{
    class ImGuiRenderer
    {
    public:
        void OnRender();

        enum class FileInteractionState
        {
            NONE         = 0,
            OPENFILE     = 1,
            OPENFOLDER   = 2,
            SAVEFILE     = 3,
        };

        void SetFileInteractionState(const FileInteractionState interactionState) { m_FileInteractionState = interactionState; }
    private:
        void RenderWorkspace();
        void RenderFileTree();

        void RenderFolderData(const std::map<std::filesystem::path, ProjectManager::FolderData>& folderData);
        void RenderFileData(const std::map<std::filesystem::path, ProjectManager::FileData>& fileData);
        
        void RenderOpenFile();
        void RenderOpenFolder();
        void RenderSaveFileAs();

        void RenderProjectData();
    private:
        FileInteractionState m_FileInteractionState;

        Buffer m_FileNameBuffer;
        Buffer m_FileDirectoryBuffer;
        
    };
}
