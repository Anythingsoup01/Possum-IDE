#include "psmpch.h"
#include "ImGuiRenderer.h"

#include <ImGuiFileDialog.h>

#include "FileManager.h"
#include "ImGuiCommands.h"
#include "Possum/PossumLayer.h"

namespace Ferret::Possum
{
    void ImGuiRenderer::OnRender()
    {
        switch (m_FileInteractionState)
        {
            case FileInteractionState::OPENFILE:
            {
                RenderOpenFile();
                break;
            }
            case FileInteractionState::OPENFOLDER:
            {
                RenderOpenFolder();
                break;
            }
            case FileInteractionState::SAVEFILE:
            {
                RenderSaveFileAs();
                break;
            }
            default:
                break;
        }

        RenderWorkspace();
        RenderFileTree();
    }

    void ImGuiRenderer::RenderWorkspace()
    {
        auto& fileManager = FileManager::Get();
        auto& files = fileManager.GetFiles();
        ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin("##WORKSPACE", nullptr, flags);
        {
            ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll;
            ImGui::BeginTabBar("##WORKSPACETABBAR", tabBarFlags);
            {
                ImVec2 size = ImGui::GetContentRegionAvail();
                for (auto& [filePath, data] : files)
                {
                    if (ImGui::BeginTabItem(data.Title.c_str(), &data.IsOpen, data.TabFlags))
                    {
                        ImGuiCommands::InputTextMultiline("##TEXTFIELD", data.Buf, data.Buf.capacity(), size, ImGuiInputTextFlags_AllowTabInput);
                        PossumLayer::Get()->SetCurrentFileData(data.Buf.data(), filePath);
                        data.TabFlags = ImGuiTabItemFlags_None;

                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    }
    
    void ImGuiRenderer::RenderFileTree()
    {
        ImGuiWindowFlags windowFlags;
        auto& fileManager = FileManager::Get();
        auto& files = fileManager.GetFiles();
        auto& currentProject = PossumLayer::Get()->GetProjectManager().GetProjectData();
        ImGui::Begin("##FILES", nullptr);
        {
            ImVec2 size = {ImGui::GetContentRegionAvail().x, 25};
            ImGui::BeginTabBar("##FILETABBAR");
            {
                if(ImGui::BeginTabItem("File Explorer"))
                {
                    if (PossumLayer::Get()->IsUsingProjectData())
                    {
                        if (ImGui::TreeNodeEx(currentProject.ProjectName.data(), ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            RenderFolderData(currentProject.Folders);
                            RenderFileData(currentProject.Files);
                            ImGui::TreePop();
                        }
                    }
                    else
                    {
                        for (auto& [filePath, data] : files)
                        {
                            if (ImGui::Button(data.Title.c_str(), size))
                            {
                                data.TabFlags = ImGuiTabItemFlags_SetSelected;
                            }
                        }
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    }

    void ImGuiRenderer::RenderFolderData(const std::map<std::filesystem::path, ProjectManager::FolderData>& folderData)
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

    void ImGuiRenderer::RenderFileData(const std::map<std::filesystem::path, ProjectManager::FileData>& fileData)
    {
        auto& fileManager = FileManager::Get();
        for (const auto& [filePath, fileData] : fileData)
        {
            if (ImGui::TreeNodeEx(fileData.FileName.c_str(), ImGuiTreeNodeFlags_Leaf))
            {
                if (ImGui::IsItemClicked())
                {
                    if (fileManager.Exists(filePath))
                        fileManager.GetFileData(filePath).TabFlags = ImGuiTabItemFlags_SetSelected;
                    else
                        fileManager.OpenFile(filePath);
                    }
                    ImGui::TreePop();
                }

        }
    }

    void ImGuiRenderer::RenderOpenFile()
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
                
            FileManager::Get().OpenFile(filePath);
            }
            instance->Close();
            m_FileInteractionState = FileInteractionState::NONE;
        }
    }

    void ImGuiRenderer::RenderOpenFolder()
    {
        auto& projectData = ProjectManager::Get().GetProjectData();
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
                projectData.reset();
                projectData.ProjectName = filePath.substr(lastSlash);
                projectData.ProjectPath = instance->GetCurrentPath();
                
                ProjectManager::Get().EmplaceFolderData(projectData.Folders, instance->GetCurrentPath());
                ProjectManager::Get().EmplaceFileData(projectData.Files, instance->GetCurrentPath());
            }
            instance->Close();
            m_FileInteractionState = FileInteractionState::NONE;

            if (!projectData.ProjectName.empty())
                ProjectManager::Get().SetInUse(true);
        }
    }
    
    void ImGuiRenderer::RenderSaveFileAs()
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

                        FileManager::Get().SaveFile(actualFilePath, PossumLayer::Get()->GetCurrentFileString().c_str());

                        std::stringstream key;

                        FileManager::Get().UpdateFileData(PossumLayer::Get()->GetCurrentFilePath(), m_FileNameBuffer.data(), m_FileNameBuffer.data(), PossumLayer::Get()->GetCurrentFileString(), actualFilePath);

                        m_FileInteractionState = FileInteractionState::NONE;
                    }
            
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    }


}
