#include "Ullpch.h"
#include "UiRectGradient.h"
#include "Application/Application.h"
#include "Rendering/Utils/RenderHelper.h"

using namespace Ull;

UiRectGradient::UiRectGradient(const std::string name, const glm::vec2 position, const glm::vec2 size) :
    UiBasicControl(name, position, size, UiControlType::UiRectGradient)
{
    for (auto colorData : m_gradientColors)
        colorData = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void UiRectGradient::SetColorData(std::initializer_list<GradientColorData> colors)
{
    for (const auto colorData : colors)
    {
        if (colorData.index >= 4)
        {
            ULOGE("UiRectColorGradient Error: color data at index " << colorData.index << " out of bounce!");
            continue;
        }

        m_gradientColors[colorData.index] =  RgbToHsl(colorData.color);
    }

    CreateResources();
}

void UiRectGradient::SetColorData(GradientColorData colorData)
{
    if (colorData.index >= 4)
    {
        ULOGE("UiRectColorGradient Error: color data at index " << colorData.index << " out of bounce!");
        return;
    }

    const auto hslColor = RgbToHsv(colorData.color);

    m_gradientColors[0] = glm::vec3(hslColor.x, 0.0f, 1.0f);
    m_gradientColors[1] = glm::vec3(hslColor.x, 1.0f, 1.0f);
    m_gradientColors[2] = glm::vec3(hslColor.x, 0.0f, 0.0f);
    m_gradientColors[3] = glm::vec3(hslColor.x, 1.0f, 0.0f);
    CreateResources();
}

void UiRectGradient::CreateResources()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    auto& shaderManager = Application::GetRenderer().GetShaderManager();
    m_shader = shaderManager.GetShader(ShaderTag::UI_GRADIENT_SHADER_HSV);

    struct VertexDesc
    {
        glm::vec3 position;
        glm::vec4 color;
    };

    std::array<VertexDesc, 4> vertices;
    vertices[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[1].position = glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[2].position = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, 1.0f, 0.0f);
    
    for (unsigned i = 0; i < 4; ++i)
    {
        const auto color = m_gradientColors[i];
        vertices[i].color = glm::vec4(color.r, color.g, color.b, 1.0f);
    }

    unsigned int indices[] = { 
        0, 1, 2,
        1, 3, 2 
    };

    m_layout = VertexLayout::Create({
        LayoutElement("Vertices", GraphicsDataType::FLOAT, 3),
        LayoutElement("Color", GraphicsDataType::FLOAT, 4)
    });

    m_layout->Bind();

    m_vertexBuffer = VertexBuffer::Create(sizeof(VertexDesc) * vertices.size(), reinterpret_cast<float*>(vertices.data()), GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();
}

void UiRectGradient::HandleEvent(Event *event)
{
    switch (event->GetType())
    {
    case EventType::MouseUp:
        InteractWithMouse();
        break;
    }

    UiBasicControl::HandleEvent(event);
}

void UiRectGradient::Update()
{ 
    const auto& mouse = Application::GetMouse();

    if (mouse.IsButtonPressed(Mouse::Button::LEFT))
        InteractWithMouse();
}

void UiRectGradient::Render()
{
    m_shader->Bind();

    m_shader->SetFloat4x4("modelMatrix", m_perspective * GetTransform());

    m_layout->Bind();

    Application::GetRenderer().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());

    UiElement::Render();
}

void UiRectGradient::InteractWithMouse()
{
}

glm::vec4 UiRectGradient::GetColorForRatio(const float ratio)
{
    return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
