#include "ImGuiCommands.h"

namespace Ferret::Possum
{
    bool ImGuiCommands::InputText(const char *label, Buffer &buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
         if (!ImGui::InputText(label, buf.data(), buf_size, flags, callback, user_data))
            return false;

        size_t dataLen = strlen(buf.data());
        size_t capacity = buf.capacity();

        if (dataLen >= capacity / 1.5)
        {
            auto& editState = ImGui::GetCurrentContext()->InputTextState; 
            buf.resize(capacity * 2);
            editState.ReloadUserBufAndKeepPosition();
        }
        return true;
    }

    bool ImGuiCommands::InputTextMultiline(const char *label, Buffer &buf, size_t buf_size, const ImVec2 &size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        if (!ImGui::InputTextMultiline(label, buf.data(), buf_size, size, flags, callback, user_data))
            return false;

        size_t dataLen = strlen(buf.data());
        size_t capacity = buf.capacity();

        if (dataLen >= capacity / 1.5)
        {
            auto& editState = ImGui::GetCurrentContext()->InputTextState; 
            buf.resize(capacity * 2);
            editState.ReloadUserBufAndKeepPosition();
        }
        return true;
    }
}
