#include "Ullpch.h"
#include "Layer.h"
#include "Layer/LayerManager.h"
#include "Application/Application.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

Layer::Layer(const std::string& name, const glm::uvec2 position, const glm::uvec2 size, const bool usesDepth, const NotOwner<LayerManager>& layerManager) :
    UiRenderArea(name, position, size, usesDepth),
    m_viewMatrix{ glm::ortho(0.0f, static_cast<float>(size.x), static_cast<float>(size.y), 0.0f, -1.0f, 1.0f) },
    m_layerManager{ layerManager }
{
    auto& shaderManager = Application::GetRenderer().GetShaderManager();
    m_shader = shaderManager.GetShader(ShaderTag::FRAME_DISPLAY_SHADER);
}

void Layer::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    switch (event->GetType())
    {
    case EventType::WindowResize:
        SetSize(static_cast<WindowResizeEvent*>(event)->GetVal());
        const auto size = GetSize();
        m_viewMatrix = glm::ortho(0.0f, size.x, size.y, 0.0f, -1.0f, 1.0f);
        ResizeLayout();
        break;
    }

	UiRenderArea::HandleEvent(event);
}

void Layer::Render()
{
    m_shader->Bind();
    m_shader->SetFloat4x4("viewMatrix", m_viewMatrix);

    m_frameBuffer->Bind();

    for (auto child : GetChildren())
    {
        if (child->GetType() != UiElementType::RenderArea)
        {
            ULOGW(GetName() + ": Element: " << child->GetName() << " has to be a renderArea element!");
            return;
        }

        CreateRenderArea(child->GetGlobalPosition(), child->GetSize());
        reinterpret_cast<UiRenderArea*>(child.get())->BindTargetTexture();

        m_layout->Bind();

        Application::GetRenderer().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
    }

    m_frameBuffer->Unbind();

    CreateRenderArea(GetPosition(), GetSize());
    BindTargetTexture();

    m_layout->Bind();

    auto& renderer = Application::GetRenderer();
    renderer.SetBlending(Renderer::State::ENABLE);
    renderer.DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
    renderer.SetBlending(Renderer::State::DISABLE);
}

void Layer::CreateRenderArea(const glm::vec2 position, const glm::vec2 size)
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    float vertices[] = {
        position.x, position.y, 0.0f, 0.0f, 1.0f,
        position.x + size.x, position.y, 0.0f, 1.0f, 1.0f,
        position.x, position.y + size.y, 0.0f, 0.0f, 0.0f,
        position.x + size.x, position.y + size.y, 0.0f, 1.0f, 0.0f
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