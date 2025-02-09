#include "Ullpch.h"
#include "FrameBufferRenderer.h"
#include "Application/Application.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

FrameBufferRenderer::FrameBufferRenderer()
{
    auto& shaderManager = Application::GetRenderer().GetShaderManager();
    m_shader = shaderManager.GetShader(ShaderTag::FRAME_DISPLAY_SHADER);
}

void FrameBufferRenderer::Render(UiRenderArea& uiRenderArea)
{
    const auto position = uiRenderArea.GetGlobalPosition();
    const auto size = uiRenderArea.GetSize();

    if (m_position != position || m_size != size)
    {
        m_position = position;
        m_size = size;

        CreateRenderArea();
    }

    uiRenderArea.BindTargetTexture();
    Render();
}

void FrameBufferRenderer::Render()
{
    Application::GetRenderer().SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    Application::GetRenderer().SetDepth(Renderer::State::DISABLE);
    Application::GetRenderer().Clear(Renderer::ClearBits::COLOR);
    Application::GetRenderer().SetViewPort(glm::ivec2(0, 0), m_size);

    m_shader->Bind();
    m_shader->SetFloat4x4("viewMatrix", m_viewMatrix);
    m_layout->Bind();

    Application::GetRenderer().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
}

void FrameBufferRenderer::CreateRenderArea()
{
    m_viewMatrix = glm::ortho(0.0f, m_size.x, m_size.y, 0.0f, -1.0f, 1.0f);

    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    float vertices[] = {
        m_position.x, m_position.y, 0.0f, 0.0f, 1.0f,
        m_position.x + m_size.x, m_position.y, 0.0f, 1.0f, 1.0f,
        m_position.x, m_position.y + m_size.y, 0.0f, 0.0f, 0.0f,
        m_position.x + m_size.x, m_position.y + m_size.y, 0.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2
    };

    m_layout = VertexLayout::Create({
        LayoutElement("Position", GraphicsDataType::FLOAT, 3),
        LayoutElement("TextureCoords", GraphicsDataType::FLOAT, 2)
        });

    m_layout->Bind();

    m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();

    m_vertexBuffer->Unbind();
    m_layout->Unbind();
}