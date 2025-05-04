#include "PossumLayer.h"
#include "Ferret.h"
#include "ImGuiFileDialog.h"
#include "imgui.h"


namespace Ferret::Possum
{
    PossumLayer* PossumLayer::s_Instance = nullptr;

    void PossumLayer::OnAttach()
    {
        s_Instance = this;
    }
    
    void PossumLayer::OnDetach()
    {
        s_Instance = nullptr;
    }

    void PossumLayer::OnUpdate(float ts)
    {
        auto& files = m_FileManager.GetFiles();
        for (auto& [filePath, data] : files)
        {
            if (!data.IsOpen)
            {
                Application::Get().SubmitToMainThread([this, filePath]()
                {
                    m_FileManager.RemoveFileData(filePath);
                });
            }
        }
        if (files.empty())
        {
            m_CurrentFileString = "";
            m_CurrentFilePath = "";
        }
    }

    void PossumLayer::OnUIRender()
    {
        if (m_FileInteraction)
        {
            RenderFileDialog();
        }
        RenderWorkspace();
        RenderFileTree();
    }

    void PossumLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
    }

    void PossumLayer::OpenFile()
    {
        m_FileInteraction = true;
        m_FileInteractionType = FileInteractionType::Open;
    }
    
    void PossumLayer::SaveFile()
    {
        if (!m_CurrentFilePath.empty())
            m_FileManager.SaveFile(m_CurrentFilePath, m_CurrentFileString.c_str());
    }

    bool PossumLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        auto ctrl = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
        auto shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

        switch (e.GetKeyCode())
        {
            case KeyCode::O:
            {
                if (ctrl)
                {
                    if (shift)
                        OpenFile();
                }
                break;
            }
            case KeyCode::S:
            {
                if (ctrl)
                {
                    SaveFile();
                }
            }
            default:
                break;
        }
        return false;
    }

    void PossumLayer::RenderWorkspace()
    {
        auto& files = m_FileManager.GetFiles();
        ImGuiWindowFlags windowFlags;
        ImGui::Begin("##WORKSPACE", nullptr);
        {
            ImVec2 size = ImGui::GetContentRegionAvail();
            ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll;
            ImGui::BeginTabBar("##WORKSPACETABBAR", tabBarFlags);
            {
                for (auto& [filePath, data] : m_FileManager.GetFiles())
                {
                    if (ImGui::BeginTabItem(data.Title.c_str(), &data.IsOpen, data.TabFlags))
                    {
                        ImGui::InputTextMultiline("##TEXTFIELD", data.Buffer.data(), data.Buffer.size() * 2, size, ImGuiInputTextFlags_AllowTabInput); // TODO: As it sits, the buffer doesn't resize, we really need a way to force a resize
                        m_CurrentFileString = data.Buffer.data();
                        m_CurrentFilePath = filePath;
                        if (data.TabFlags == ImGuiTabItemFlags_SetSelected)
                            data.TabFlags = ImGuiTabItemFlags_None;
                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    }

    void PossumLayer::RenderFileTree()
    {
        ImGuiWindowFlags windowFlags;
        auto& files = m_FileManager.GetFiles();
        ImGui::Begin("##FILES", nullptr);
        {
            ImVec2 size = {ImGui::GetContentRegionAvail().x, 25};
            ImGui::BeginTabBar("##FILETABBAR");
            {
                if(ImGui::BeginTabItem("File Explorer"))
                {
                    for (auto& [filePath, data] : files)
                    {
                        if (ImGui::Button(data.Title.c_str(), size))
                        {
                            data.TabFlags = ImGuiTabItemFlags_SetSelected;
                        }
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    }

    void PossumLayer::RenderFileDialog()
    {
        switch(m_FileInteractionType)
        {
            case FileInteractionType::Open:
            {
                RenderOpenFile();
                break;
            }
            case FileInteractionType::Save:
            {
                RenderSaveFileAs();
                break;
            }
            default:
                m_FileInteraction = false;
        }
    }

    void PossumLayer::RenderOpenFile()
    {
        const auto& instance = ImGuiFileDialog::Instance();
        ImGui::Begin("##OpenFileDialog");
        {
            // We use a stringstream as we
            // may want a way to get the
            // username for ease of use
            std::stringstream ss;
            ss << "/home/";
            IGFD::FileDialogConfig config; config.path = ss.str();
            instance->OpenDialog("OpenFileDialog", "Choose File", ".c, .cpp, .h, .hpp", config);
            
            ImGui::End();
        }
        
        if (instance->Display("OpenFileDialog", ImGuiWindowFlags_NoCollapse))
        {
            if (instance->IsOk())
            {
                std::filesystem::path filePath = instance->GetFilePathName();
                
                m_FileManager.OpenFile(filePath);
            }
            instance->Close();
            m_FileInteractionType = FileInteractionType::None;
        }

    }

    void PossumLayer::RenderSaveFileAs()
    {
        ImGui::Begin("##SAVEFILEAS");
        {
            ImGui::InputText("Save File As", m_FileNameBuffer, 128);
            if (Ferret::Input::IsKeyPressed(KeyCode::Enter))
                m_FileManager.SaveFile(m_FileNameBuffer, m_CurrentFileString.c_str());
            ImGui::End();
        }
        m_FileInteractionType = FileInteractionType::None;
    }
}
