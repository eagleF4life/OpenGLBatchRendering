#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class SandboxLayer : public GLCore::Layer
{
public:
    SandboxLayer();
    virtual ~SandboxLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(GLCore::Event& event) override;
    void OnUpdate(GLCore::Timestep ts) override;
    void OnImGuiRender() override;

    private:
    std::unique_ptr<GLCore::Utils::Shader> m_Shader;
    GLCore::Utils::OrthographicCameraController m_CameraController;

    GLuint m_QuadVA, m_QuadVB, m_QuadIB;

    glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
    glm::vec4 m_SquareAlternateColor = { 0.2f, 0.3f, 0.8f, 1.0f };
    glm::vec4 m_SquareColor = { 1.0f, 1.0f, 1.0f, 1.0f };
};