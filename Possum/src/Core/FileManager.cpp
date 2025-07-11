#include "psmpch.h"
#include "FileManager.h"
#include "Ferret/Core/Application.h"
#include "FileInteraction.h"

namespace Ferret::Possum
{
    FileManager* FileManager::s_Instance = nullptr;

    FileManager::FileManager()
    {
        s_Instance = this;
    }

    FileManager::~FileManager()
    {
        for (int i = 0; i <= m_UntitledDocuments; i++)
        {
            std::stringstream ss;
            ss << "/tmp/possum/UntitledDocument" << i;
            std::remove(ss.str().c_str());
        }
        s_Instance = nullptr;
    }

    void FileManager::NewFile()
    {
        std::stringstream filePath;
        filePath << "/tmp/possum/";
        std::stringstream fileName;
        fileName << "UntitledDocument" << m_UntitledDocuments;
        
        if (!std::filesystem::exists(filePath.str()))
            std::filesystem::create_directories(filePath.str());
        std::stringstream ss;
        ss << filePath.str() << fileName.str();
        std::ofstream out(ss.str());
        if (!out.is_open())
        {
            FE_CLI_ERROR("Failed to create new file!");
            return;
        }

        FileData fileData;
        fileData.Title = fileName.str();
        fileData.IsUntitled = true;
        fileData.IsOpen = true;
        fileData.Buf.copy("\0\0\0\0\0\0\0\0\0\0");

        InsertFileData(ss.str(), fileData);

        m_UntitledDocuments++;
    }
    
    void FileManager::OpenFile(const std::filesystem::path& filePath)
    {
        std::ifstream in(filePath);
        if (!in.is_open())
        {
            FE_CLI_CRITICAL("Failed to open file!");
            return;
        }
        
        std::string defaultFileName = filePath.filename().string();

        FileData fileData;
        fileData.Title = defaultFileName;
        fileData.AltTitle = defaultFileName; // TODO : Have this captcher the directory before as well
        fileData.IsUntitled = false;
        fileData.IsOpen = true;
        
        std::string inData = FileInteraction::ReadFile(filePath);
        fileData.Buf.copy(inData.data());

        InsertFileData(filePath, fileData);
    }

    void FileManager::SaveFile(const std::filesystem::path& filePath, const std::string& outData)
    {
        FileInteraction::WriteFile(filePath, outData);
    }

    void FileManager::UpdateFileData(const std::filesystem::path& key, const std::string& title, const std::string& altTitle, const std::string& fileString, const std::filesystem::path& newKey)
    {
        Application::Get().SetMenubarCallback([this, key](){
            RemoveFileData(key);
        });
        FileData fileData;
        fileData.Title = title;
        fileData.AltTitle = altTitle;
        fileData.IsUntitled = false;
        fileData.IsOpen = true;
        fileData.TabFlags = ImGuiTabItemFlags_SetSelected;
        fileData.Buf.copy(fileString.c_str());

        InsertFileData(newKey, fileData);
    }

    void FileManager::InsertFileData(const std::filesystem::path& key, const FileData fileData)
    {
        m_Files.emplace(std::pair<std::filesystem::path, FileData>(key, fileData));
    }

    void FileManager::RemoveFileData(const std::filesystem::path& key)
    {
        m_Files.erase(key);
    }

    FileData& FileManager::GetFileData(const std::filesystem::path& key)
    {
        return m_Files.at(key);
    }

    bool FileManager::Exists(const std::filesystem::path& key)
    {
        for (auto& [storedKey, data] : m_Files)
        {
            if (key == storedKey)
                return true;
        }
        return false;
    }
}
