#include "Ullpch.h"
#include "UiLinearColorGradient.h"
#include "Application/Application.h"
#include "Utilities/CollisionCheckers.h"

using namespace Ull;

UiLinearColorGradient::UiLinearColorGradient(const std::string name, const glm::vec2 position, const glm::vec2 size, const UiLinearColorGradient::GradientDirection gradientDirection) :
    UiBasicControl(name, position, size, UiControlType::UiLinearGradient),
    m_gradientDirection{ gradientDirection }
{   
}

void UiLinearColorGradient::AddColor(const GradientColorData colorData)
{
    m_gradientColors.push_back(colorData);
}

void UiLinearColorGradient::ClearColorData()
{
    m_gradientColors.clear();
}

void UiLinearColorGradient::CreateResources()
{
    std::sort(m_gradientColors.begin(), m_gradientColors.end(), [](const GradientColorData& dataPoint1, const GradientColorData& dataPoint2) {
        return dataPoint1.position < dataPoint2.position;
    });

    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    auto& shaderManager = Application::GetRenderer().GetShaderManager();

    if (m_gradientColors.size() < 2)
    {   
        m_shader = shaderManager.GetShader(ShaderTag::UI_BASIC_COLOR);
        UiBasicControl::CreateResources();

        if(!m_gradientColors.empty())
            SetBackgroundColor(m_gradientColors[0].color);

        return;
    }

    m_shader = shaderManager.GetShader(ShaderTag::UI_GRADIENT_SHADER);

    struct VertexDesc 
    {
        glm::vec3 position;
        glm::vec4 color;
    };

    std::vector<VertexDesc> vertices;
    std::vector<unsigned> indices;
    unsigned index = 0;

    //DOTO: Check those reserve calls
    vertices.reserve((m_gradientColors.size() + 1) * 2);
    indices.reserve((m_gradientColors.size() + 2) * 4);

    const auto drawDirection = m_gradientDirection == GradientDirection::HORIZONTAL ? glm::vec2(1.0f, 0.0f) : glm::vec2(0.0f, 1.0f);
    const auto normalToDrawDirection = m_gradientDirection == GradientDirection::HORIZONTAL ? glm::vec2(0.0f, 1.0f) : glm::vec2(1.0f, 0.0f);

    if (m_gradientColors[0].position > 0.0f)
    {
        auto firstColor = m_gradientColors[0];
        firstColor.position = 0.0f;
        m_gradientColors.insert(m_gradientColors.begin(), firstColor);
    }

    const auto colorCount = m_gradientColors.size();
    if (m_gradientColors[colorCount - 1].position < 1.0f)
    {
        auto lastColor = m_gradientColors[colorCount - 1];
        lastColor.position = 1.0f;
        m_gradientColors.push_back(lastColor);
    }

    for (size_t i = 0; i < m_gradientColors.size(); ++i)
    {
        const auto colorData = m_gradientColors[i];

        if (colorData.position > 1.0f || colorData.position < 0.0f)
        {
            ULOGE("UiLinearColorGradient Error: color data " << i << " out of bounce!");
            continue;
        }

        const auto drawingStep = drawDirection * colorData.position;
        const auto normalDrawingStep = normalToDrawDirection + drawingStep;
        
        vertices.push_back(VertexDesc{ glm::vec3(drawingStep.x, drawingStep.y, 0.0f), colorData.color });
        
        if (index > 2)
        {
            indices.push_back(index - 2);
            indices.push_back(index - 1);
        }
        indices.push_back(index++);

        vertices.push_back(VertexDesc{ glm::vec3(normalDrawingStep.x, normalDrawingStep.y, 0.0f), colorData.color });

        if (index > 1)
        {
            indices.push_back(index - 2);
            indices.push_back(index - 1);
        }

        indices.push_back(index++);
    }

    m_layout = VertexLayout::Create({
        LayoutElement("Vertices", GraphicsDataType::FLOAT, 3),
        LayoutElement("Color", GraphicsDataType::FLOAT, 4)
    });

    m_layout->Bind();


    m_vertexBuffer = VertexBuffer::Create(sizeof(VertexDesc) * static_cast<int>(vertices.size()), reinterpret_cast<float*>(vertices.data()), GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(unsigned) * static_cast<int>(indices.size()), indices.data(), GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();
}

void UiLinearColorGradient::HandleEvent(Event *event)
{
    switch (event->GetType())
    {
    case EventType::MouseUp:
        if (IsVisible() == false)
            return;

        InteractWithMouse();
        break;
    }

    UiBasicControl::HandleEvent(event);
}

void UiLinearColorGradient::Update()
{
    const auto& mouse = Application::GetMouse();

    if (mouse.IsButtonPressed(Mouse::Button::LEFT))
        InteractWithMouse();
}

void UiLinearColorGradient::Render()
{
    m_shader->Bind();

    m_shader->SetFloat4x4("modelMatrix", m_perspective * GetTransform());

    m_layout->Bind();

    Application::GetRenderer().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());

    UiElement::Render();
}

void UiLinearColorGradient::InteractWithMouse()
{
    if (!m_isInteractive)
        return;

    const auto mousePosition = glm::vec2(Application::GetMouse().GetMousePosition());
    const auto controlPosition = GetGlobalPosition();
    const auto controlSize = GetSize() * GetScale();

    if (PointInStaticRect(mousePosition, controlPosition, controlSize))
    {
        const auto inRectMousePosition = mousePosition - controlPosition;
         const auto ratio = m_gradientDirection == GradientDirection::HORIZONTAL ?
                            inRectMousePosition.x / controlSize.x :
                            inRectMousePosition.y / controlSize.y;

        m_lastInteractionColor = GetColorForRatio(ratio);
    }
}

glm::vec4 UiLinearColorGradient::GetColorForRatio(const float ratio)
{
    for (auto itr = m_gradientColors.begin(); itr != m_gradientColors.end(); ++itr)
    {
        if (itr->position >= ratio)
        {
            if (itr == m_gradientColors.begin())
                return itr->color;
            
            const auto previousColorDataPoint = itr - 1;
            const auto relativeRatio = ratio - previousColorDataPoint->position;
            const auto range = itr->position - previousColorDataPoint->position;
            if (range <= 0.0f)
                itr->color;

            const auto rangeRatio = relativeRatio / range;
            return (previousColorDataPoint->color * (1.0f - rangeRatio)) + (itr->color * rangeRatio);
        }
    }

    return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}