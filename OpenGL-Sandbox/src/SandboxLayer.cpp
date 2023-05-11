#include "SandboxLayer.h"
#include "stb_image/stb_image.h"

using namespace GLCore;
using namespace GLCore::Utils;

struct Vertex
{
    std::array<float, 3> Position;
    std::array<float, 4> Color;
    std::array<float, 2> TexCoords;
    float TexID;
};

SandboxLayer::SandboxLayer() : m_CameraController(16.0f / 9.0f)
{
}

SandboxLayer::~SandboxLayer()
{
}

static GLuint LoadTexture(const std::string& path)
{
    int width;
    int height;
    int bitsPerPixel;

    // Load image png file
    stbi_set_flip_vertically_on_load(1);
    auto* pixels = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, STBI_rgb_alpha);
    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);

    return textureID;
}

void SandboxLayer::OnAttach()
{
    EnableGLDebugging();

    m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
        "assets/shader.glsl.vert",
        "assets/shader.glsl.frag"
    ));

    // Enable Blending
    glEnable(GL_BLEND);
    // We are defining how open GL is going to blend alpha pixels
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_Shader->GetRendererID());
    const auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
    int samplers[2] = { 0, 1 };
    glUniform1iv(loc, 2, samplers);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    const size_t MaxQuadCount = 1000;
    const size_t MaxVertexCount = MaxQuadCount * 4;
    const size_t MaxIndexCount = MaxQuadCount * 6;

    // Vertex Array
    glCreateVertexArrays(1, &m_QuadVA);
    glBindVertexArray(m_QuadVA);

    // Vertex Buffer
    glCreateBuffers(1, &m_QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // Setup Vertex Attribute Pointer - For position
    glEnableVertexArrayAttrib(m_QuadVB, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Position)));

    // For color
    glEnableVertexArrayAttrib(m_QuadVB, 1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Color)));

    // For texture
    glEnableVertexArrayAttrib(m_QuadVB, 2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, TexCoords)));

    // For texture Id
    glEnableVertexArrayAttrib(m_QuadVB, 3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, TexID)));

    /*uint32_t indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };*/

    uint32_t indices[MaxIndexCount];
    uint32_t offset = 0;
    for (size_t i = 0; i < MaxIndexCount; i += 6)
    {
        indices[i] = offset;
        indices[i+1] = 1 + offset;
        indices[i+2] = 2 + offset;
        indices[i+3] = 2 + offset;
        indices[i+4] = 3 + offset;
        indices[i+5] = offset;

        offset += 4;
    }

    // Index Buffer
    glCreateBuffers(1, &m_QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    m_ArsenalTex = LoadTexture("assets/Arsenal.png");
    m_BenficaTex = LoadTexture("assets/Benfica.png");
}

void SandboxLayer::OnDetach()
{
    // Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
    m_CameraController.OnEvent(event);
}

static Vertex* CreateQuad(Vertex* target, float x, float y, float textureID)
{
    const float size = 1.0f;

    target->Position = { x, y, 0.0f };
    target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    target->TexCoords = { 0.0f, 0.0f };
    target->TexID = textureID;
    target++;

    target->Position = { x + size, y, 0.0f };
    target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    target->TexCoords = { 1.0f, 0.0f };
    target->TexID = textureID;
    target++;

    target->Position = { x + size,  y + size, 0.0f };
    target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    target->TexCoords = { 1.0f, 1.0f };
    target->TexID = textureID;
    target++;

    target->Position = { x,  y + size, 0.0f };
    target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    target->TexCoords = { 0.0f, 1.0f };
    target->TexID = textureID;
    target++;

    return target;
}

void SandboxLayer::OnUpdate(Timestep ts)
{
    // This function runs every frame

    m_CameraController.OnUpdate(ts);

    /*float vertices[] = {
        -1.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f, // position X,Y,Z; RGBa; Texture Coordinates, Texture Id - 10 floats
        -0.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
        -1.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

         0.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
         1.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,
    };*/

    uint32_t indexCount = 0;
    std::array<Vertex, 1000> vertices{};
    Vertex* buffer = vertices.data();

    // Create a 5 by 5 array of quads
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            buffer = CreateQuad(buffer, float(x), float(y), float((x + y) % 2));
            indexCount += 6;
        }
    }

    CreateQuad(buffer, m_QuadAPosition[0], m_QuadAPosition[1], 0.0f);
    indexCount += 6;

    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_Shader->GetRendererID());
    glBindTextureUnit(0, m_ArsenalTex);
    glBindTextureUnit(1, m_BenficaTex);

    // Setup uniforms
    const auto& vpMat = m_CameraController.GetCamera().GetViewProjectionMatrix();
    m_Shader->SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProj", vpMat);
    m_Shader->SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    //m_Shader->SetUniform4fv(m_Shader->GetRendererID(), "u_Color", m_SquareColor);

    glBindVertexArray(m_QuadVA);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::OnImGuiRender()
{
    // ImGui here
    ImGui::Begin("Controls");
    ImGui::DragFloat2("Quad A Position", m_QuadAPosition, 0.1f);
    ImGui::DragFloat2("Quad B Position", m_QuadBPosition, 0.1f);
    ImGui::End();
}