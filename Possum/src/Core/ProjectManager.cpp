#include <psmpch.h>
#include "ProjectManager.h"

namespace Ferret::Possum
{
    ProjectManager* ProjectManager::s_Instance = nullptr;

    ProjectManager::ProjectManager()
    {
        s_Instance = this;
    }
    
    ProjectManager::~ProjectManager()
    {
        s_Instance = nullptr;
    }

    void ProjectManager::EmplaceFolderData(std::map<std::filesystem::path, FolderData>& folderData, const std::filesystem::path& folderPath)
    {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            if (entry.is_directory())
            {
                FolderData folder;
                std::string name = entry.path().string();
                size_t lastSlash = name.find_last_of('/') + 1;
                folder.FolderName = name.substr(lastSlash);
                EmplaceFileData(folder.Files, entry.path());
                EmplaceFolderData(folder.SubFolders, entry.path());

                folderData.emplace(std::pair<std::filesystem::path, FolderData>(entry.path(), folder));
            }
        }
    }

    void ProjectManager::EmplaceFileData(std::map<std::filesystem::path, FileData>& fileData, const std::filesystem::path& folderPath)
    {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            if (entry.is_regular_file())
            {
                FileData file;
                std::string name = entry.path().string();
                size_t lastSlash = name.find_last_of('/') + 1;
                file.FileName = name.substr(lastSlash);

                fileData.emplace(std::pair<std::filesystem::path, FileData>(entry.path(), file));
            }


        }
    }

}
