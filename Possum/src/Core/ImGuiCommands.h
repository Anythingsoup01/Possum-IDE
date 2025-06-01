#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include "Buffer.h"

namespace Ferret::Possum
{
    class ImGuiCommands
    {
    public:
        static bool InputText(const char* label, Buffer& buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
        static bool InputTextMultiline(const char* label, Buffer& buf, size_t buf_size, const ImVec2& size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);   };
}
