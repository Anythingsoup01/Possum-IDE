#include "PossumLayer.h"
#include "Ferret.h"
#include "ImGuiFileDialog.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "Core/ImGuiCommands.h"

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

    void PossumLayer::NewFile()
    {
        m_FileManager.NewFile();
    }

    void PossumLayer::OpenFile()
    {
        m_FileInteraction = true;
        m_FileInteractionType = FileInteractionType::Open;
    }

    void PossumLayer::OpenFolder()
    {
        m_FileInteraction = true;
        m_FileInteractionType = FileInteractionType::Folder;
    }
    
    void PossumLayer::SaveFile()
    {
        if (!m_CurrentFilePath.empty() && !m_FileManager.GetFileData(m_CurrentFilePath).IsUntitled)
            m_FileManager.SaveFile(m_CurrentFilePath, m_CurrentFileString.c_str());

        else if (m_FileManager.GetFileData(m_CurrentFilePath).IsUntitled)
        {
            m_FileInteractionType = FileInteractionType::Save;
            m_FileInteraction = true;
        }
    }

    bool PossumLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        auto ctrl = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
        auto shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

        switch (e.GetKeyCode())
        {
            case KeyCode::N:
            {
                if (ctrl)
                    NewFile();
                break;
            }
            case KeyCode::O:
            {
                if (ctrl)
                {
                    if (shift)
                    {
                        OpenFolder();
                    }
                    else 
                    {
                        OpenFile();
                    }
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
        ImGui::Begin("##WORKSPACE", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll;
            ImGui::BeginTabBar("##WORKSPACETABBAR", tabBarFlags);
            {
                ImVec2 size = ImGui::GetContentRegionAvail();
                for (auto&& [filePath, data] : m_FileManager.GetFiles())
                {
                    if (ImGui::BeginTabItem(data.Title.c_str(), &data.IsOpen, data.TabFlags))
                    {
                    ImGuiCommands::InputTextMultiline("##TEXTFIELD", data.Buffer, data.Buffer.capacity(), size, ImGuiInputTextFlags_AllowTabInput);
                        m_CurrentFileString = data.Buffer.data();
                        m_CurrentFilePath = filePath;
                        m_CurrentFileName = data.Title;
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
                    if (!m_UsingProjectData)
                    {
                        for (auto& [filePath, data] : files)
                        {
                            if (ImGui::Button(data.Title.c_str(), size))
                            {
                                data.TabFlags = ImGuiTabItemFlags_SetSelected;
                            }
                        }
                    }
                    else
                    {
                        if (ImGui::TreeNodeEx(m_ProjectData.ProjectName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            RenderFolderData(m_ProjectData.Folders);
                            RenderFileData(m_ProjectData.Files);
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    }

    void PossumLayer::RenderFolderData(const std::map<std::filesystem::path, FolderData>& folderData)
    {
        for (const auto& [filePath, data] : folderData)
        {
            if (ImGui::TreeNodeEx(data.FolderName.c_str()))
            {
                if (!data.SubFolders.empty())
                    RenderFolderData(data.SubFolders);

                RenderFileData(data.Files);

                ImGui::TreePop();
            }
        }
    }

    void PossumLayer::RenderFileData(const std::map<std::filesystem::path, FileData>& fileData)
    {
        for (const auto& [filePath, fileData] : fileData)
        {
            if (ImGui::TreeNodeEx(fileData.FileName.c_str(), ImGuiTreeNodeFlags_Leaf))
            {
                if (ImGui::IsItemClicked())
                {
                    if (m_FileManager.Exists(filePath))
                        m_FileManager.GetFileData(filePath).TabFlags = ImGuiTabItemFlags_SetSelected;
                    else
                        m_FileManager.OpenFile(filePath);
                    }
                    ImGui::TreePop();
                }

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
            case FileInteractionType::Folder:
            {
                RenderOpenFolder();
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
        
        if (instance->Display("OpenFileDialog"))
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

    void PossumLayer::RenderOpenFolder()
    {
        const auto& instance = ImGuiFileDialog::Instance();
        ImGui::Begin("##OpenFolderDialog");
        {
            IGFD::FileDialogConfig config; config.path = "/home/";
            instance->OpenDialog("OpenFolderDialog", "Choose Directory", nullptr, config);
            ImGui::End();
        }

        if (instance->Display("OpenFolderDialog"))
        {
            if (instance->IsOk())
            {
                std::string filePath = instance->GetCurrentPath();
                size_t lastSlash = filePath.find_last_of('/') + 1;
                m_ProjectData = ProjectData();
                m_ProjectData.ProjectName = filePath.substr(lastSlash);
                m_ProjectData.ProjectPath = instance->GetCurrentPath();
                
                EmplaceFolderData(m_ProjectData.Folders, instance->GetCurrentPath());
                EmplaceFileData(m_ProjectData.Files, instance->GetCurrentPath());
            }
            instance->Close();
            m_FileInteractionType = FileInteractionType::None;

            if (!m_ProjectData.ProjectName.empty())
                m_UsingProjectData = true;
        }
    }

    void PossumLayer::EmplaceFolderData(std::map<std::filesystem::path, FolderData>& folderData, const std::filesystem::path& folderPath)
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

    void PossumLayer::EmplaceFileData(std::map<std::filesystem::path, FileData>& fileData, const std::filesystem::path& folderPath)
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

    void PossumLayer::RenderSaveFileAs()
    {
        ImGui::Begin("##SAVEFILEAS");
        {
            ImGui::BeginTabBar("##SaveFile");
            {
                if (ImGui::BeginTabItem("SaveFile"))
                {
                    ImGuiCommands::InputText("Save File As", m_FileNameBuffer, 64);
                    ImGuiCommands::InputText("Directory", m_FileDirectoryBuffer, 256);

                    if (Input::IsKeyPressed(KeyCode::Enter) && m_FileNameBuffer.active() && m_FileDirectoryBuffer.active())
                    {
                        int lastCharIndex = strlen(m_FileDirectoryBuffer.data());

                        if (m_FileDirectoryBuffer[lastCharIndex - 1] != '/')
                            m_FileDirectoryBuffer.set(lastCharIndex, '/');

                        std::string actualFilePath(m_FileDirectoryBuffer.data());
                        actualFilePath.append(m_FileNameBuffer.data());

                        m_FileManager.SaveFile(actualFilePath, m_CurrentFileString.c_str());

                        std::stringstream key;

                        m_FileManager.UpdateFileData(m_CurrentFilePath, m_FileNameBuffer.data(), m_FileNameBuffer.data(), m_CurrentFileString, actualFilePath);

                        m_FileInteractionType = FileInteractionType::None;
                    }
            
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
        
    }
}
