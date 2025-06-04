#pragma once

#include <unordered_map>
#include <filesystem>
#include <imgui.h>

#include "Buffer.h"

namespace Ferret::Possum
{
    struct FileData
    {
        std::string Title;
        std::string AltTitle;
        std::filesystem::path FilePath;

        bool IsUntitled = false;
        bool IsOpen = false;
    
        Buffer Buf;

        ImGuiTabItemFlags TabFlags = ImGuiTabItemFlags_None;
    };

    class FileManager
    {
    public:
        FileManager();
        ~FileManager();

        void NewFile();
        void OpenFile(const std::filesystem::path& filePath);
        void SaveFile(const std::filesystem::path& filePath, const std::string& outData);

        void UpdateFileData(const std::filesystem::path& key, const std::string& title, const std::string& altTitle, const std::string& fileString, const std::filesystem::path& newKey);
        void InsertFileData(const std::filesystem::path& key, const FileData fileData);
        void RemoveFileData(const std::filesystem::path& key);

        FileData& GetFileData(const std::filesystem::path& key);
        bool Exists(const std::filesystem::path& key);

        std::unordered_map<std::filesystem::path, FileData>& GetFiles() { return m_Files; }

        static FileManager& Get() { return *s_Instance; }
    private:
        static FileManager* s_Instance;
        std::unordered_map<std::filesystem::path, FileData> m_Files;

        int m_UntitledDocuments = 0;
    };
}
