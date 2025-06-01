#include "psmpch.h"
#include "PossumLayer.h"

namespace Ferret::Possum
{
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
        m_ImGuiRenderer.OnRender();
    }

    void PossumLayer::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(PossumLayer::OnKeyPressed));
    }

    void PossumLayer::NewFile()
    {
        m_FileManager.NewFile();
    }

    void PossumLayer::OpenFile()
    {
        m_ImGuiRenderer.SetFileInteractionState(ImGuiRenderer::FileInteractionState::OPENFILE);
    }
    
    void PossumLayer::OpenFolder()
    {
        m_ImGuiRenderer.SetFileInteractionState(ImGuiRenderer::FileInteractionState::OPENFOLDER);
    }

    void PossumLayer::SaveFile()
    {
        if (!m_CurrentFilePath.empty() && !m_FileManager.GetFileData(m_CurrentFilePath).IsUntitled)
            m_FileManager.SaveFile(m_CurrentFilePath, m_CurrentFileString.c_str());

        else if (m_FileManager.GetFileData(m_CurrentFilePath).IsUntitled)
        {
            m_ImGuiRenderer.SetFileInteractionState(ImGuiRenderer::FileInteractionState::SAVEFILE);
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
}
