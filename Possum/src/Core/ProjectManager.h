#pragma once



namespace Ferret::Possum
{
    class ProjectManager
    {
    public:
        // This is a struct primarily for future functionality
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

            const void reset()
            {
                this->ProjectName = "";
                this->ProjectPath = "";
                this->Folders.clear();
                this->Files.clear();
            }
        };

    public:
        ProjectManager();
        ~ProjectManager();

        // If the user opens a folder, it will
        // still open it as a project, just
        // without any project configurations
        void OpenProject(const std::filesystem::path& projectPath);

        void EmplaceFolderData(std::map<std::filesystem::path, FolderData>& folderData, const std::filesystem::path& folderPath);
        void EmplaceFileData(std::map<std::filesystem::path, FileData>& fileData, const std::filesystem::path& folderPath);

        ProjectData& GetProjectData() { return m_ProjectData; }

        static ProjectManager& Get() { return *s_Instance; }

        void SetInUse(bool inUse) { m_InUse = inUse; }

        bool InUse() { return m_InUse; }

    private:

    private:
        void EmplaceFolderData(const std::map<std::filesystem::path, FolderData>& folderData);
        void EmplaceFileData(const std::map<std::filesystem::path, FileData>& fileData);
        
    private: 
        static ProjectManager* s_Instance;
        
        // TODO: Set up an unordered_map of these and keep track of current project data opened
        ProjectData m_ProjectData;

        bool m_InUse = false;
    };
}
