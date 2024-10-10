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
    auto& shaderManager = Renderer::GetInstance().GetShaderManager();
	m_shader = shaderManager.GetShader(ShaderTag::FRAME_DISPLAY_SHADER);
	CreateResources();
}

UiLayout::~UiLayout()
{

}

void UiLayout::CreateResources()
{
    const auto size = GetSize();
    m_viewMatrix = glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, -1.0f, 1.0f);
}

void UiLayout::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    for (const auto& uiElement : GetChildren())
    {
        uiElement->HandleEvent(event);
    }
}

void UiLayout::Update()
{

}

void UiLayout::Render()
{
    const auto size = GetSize();

    Renderer::GetInstance().SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    Renderer::GetInstance().SetDepth(Renderer::State::DISABLE);
    Renderer::GetInstance().Clear(Renderer::ClearBits::COLOR);
    Renderer::GetInstance().SetViewPort(glm::ivec2(0, 0), size);

    Renderer::GetInstance().SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

    m_shader->Bind();
    m_shader->SetFloat4x4("viewMatrix", m_viewMatrix);

    const auto elementCount = GetChildren().size();

    for (int i = 0; i < elementCount; ++i)
    {
        CreateRenderAreaForUiElements(i);
        m_layout->Bind();
        static_cast<UiArea*>(GetChildren()[i].get())->BindTargetTexture();
        Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
    }
}

void UiLayout::CreateRenderAreaForUiElements(unsigned int elementID)
{
    const auto element = GetChildren()[elementID];

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