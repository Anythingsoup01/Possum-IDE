#pragma once

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

        std::string Content; // TODO: Maybe delete this, buffer already provides us with the string data!
        CBuffer Buffer;

        ImGuiTabItemFlags TabFlags = ImGuiTabItemFlags_None;
    };

    class FileManager
    {
    public:
        void OpenFile(const std::filesystem::path& filePath);
        void SaveFile(const std::filesystem::path& filePath, const char* outData);

        void InsertFileData(const std::filesystem::path& key, const FileData fileData);
        void RemoveFileData(const std::filesystem::path& key);

        FileData& GetFileData(const std::filesystem::path& key);
        bool Exists(const std::filesystem::path& key);

        std::unordered_map<std::filesystem::path, FileData> GetFiles() const { return m_Files; }
    private:
        std::unordered_map<std::filesystem::path, FileData> m_Files;
    };
}
