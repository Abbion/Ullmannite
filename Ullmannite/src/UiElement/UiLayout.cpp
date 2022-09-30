#include "Ullpch.h"
#include "UiLayout.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "UiArea.h"

using namespace Ull;

UiLayout::UiLayout(std::string name, glm::uvec2 position, glm::uvec2 size) :
	UiElement(name, position, size)
{
	m_shader = ShaderManager::GetInstance().GetShader(ShaderTag::FRAME_DISPLAY_SHADER);
	CreateResources();
}

UiLayout::~UiLayout()
{
    
}

void UiLayout::CreateResources()
{
    m_viewMatrix = glm::ortho(0.0f, (float)m_size.x, (float)m_size.y, 0.0f, -1.0f, 1.0f);
}

void UiLayout::HandleEvent(Event* event)
{
    for (const auto& uiElement : m_children)
    {
        uiElement->HandleEvent(event);
    }
}

void UiLayout::Update()
{

}

void UiLayout::Render()
{
    Renderer::GetInstance().SetClearColor(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f)); //TODO: add as debug
    Renderer::GetInstance().SetDepth(Renderer::State::DISABLE);
    Renderer::GetInstance().Clear(Renderer::ClearBits::COLOR);
    Renderer::GetInstance().SetViewPort(glm::ivec2(0, 0), m_size);

    Renderer::GetInstance().SetClearColor(glm::vec4(0.5f, 0.0f, 0.5f, 1.0f)); //TODO: add as debug

    m_shader->Bind();
    m_shader->SetFloat4x4("viewMatrix", m_viewMatrix);

    const auto elementCount = m_children.size();

    for (int i = 0; i < elementCount; ++i)
    {
        CreateRenderAreaForUiElements(i);
        m_layout->Bind();
        static_cast<UiArea*>(m_children[i])->BindTargetTexture();
        Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
    }
}

void UiLayout::CreateRenderAreaForUiElements(unsigned int elementID)
{
    const auto element = m_children[elementID];

    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    glm::vec2 pos = glm::vec2(element->GetPosition()) * element->GetScale();
    glm::vec2 size = glm::vec2(element->GetPosition() + element->GetSize()) * element->GetScale();

    float vertices[] = {
        pos.x, pos.y, 0.0f,     0.0f, 1.0f,
        size.x, pos.y, 0.0f,    1.0f, 1.0f,
        pos.x, size.y, 0.0f,    0.0f, 0.0f,
        size.x, size.y, 0.0f,    1.0f, 0.0f
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