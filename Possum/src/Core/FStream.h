#pragma once

namespace Ferret::Possum
{
    class FileInteraction
    {
    public:
        static std::string ReadFile(const std::filesystem::path& filePath);
        static void WriteFile(const std::filesystem::path& filePath, const std::string& outData);
    };
}
