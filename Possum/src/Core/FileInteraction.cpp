#include "psmpch.h"
#include "FileInteraction.h"

namespace Ferret::Possum
{
    std::string FileInteraction::ReadFile(const std::filesystem::path& filePath)
    {
        std::ifstream in(filePath);
        if (!in.is_open())
        {
            FE_CLI_ERROR("Failed to open file at {}!", filePath.string());
            return std::string();
        }
        std::stringstream ss;
        ss << in.rdbuf();
        return ss.str();
    }

    void FileInteraction::WriteFile(const std::filesystem::path &filePath, const std::string &outData)
    {
        std::ofstream out(filePath);
        if (!out.is_open())
        {
            FE_CLI_ERROR("Failed to open file at {}!", filePath.string());
            return;
        }
        out << outData.data();
    }
}
