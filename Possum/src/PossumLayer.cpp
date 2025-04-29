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

        // Dispatch events
    }

    void PossumLayer::OpenFile()
    {
        m_FileInteraction = true;
        m_FileInteractionType = FileInteractionType::Open;
    }
    
    void PossumLayer::SaveFile()
    {
        m_FileInteraction = true;
        m_FileInteractionType = FileInteractionType::Save;
    }

    void PossumLayer::RenderWorkspace()
    {
        ImGuiWindowFlags windowFlags;
        ImGui::Begin("##WORKSPACE", nullptr);
        {
            
            ImGui::End();
        }
    }

    void PossumLayer::RenderFileTree()
    {
        ImGuiWindowFlags windowFlags;
        ImGui::Begin("File Explorer", nullptr);
        {
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
                RenderSaveFile();
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
                std::string bufStr(m_FileManager.GetFileData(filePath).Buffer.data());
                std::cout << m_FileManager.GetFileData(filePath).Content << std::endl;
                //std::cout << bufStr << std::endl;
            }
            instance->Close();
            m_FileInteractionType = FileInteractionType::None;
        }

    }

    void PossumLayer::RenderSaveFile()
    {
        
    }
}
