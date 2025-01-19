#include "Ullpch.h"
#include "UiColorGradient.h"
#include "Application/Application.h"

using namespace Ull;

UiColorGradient::UiColorGradient(std::string name, glm::vec2 position, glm::vec2 size, UiColorGradient::GradientDirection gradientDirection) :
    UiBasicControl(name, position, size, UiControlType::UiGradient),
    m_gradientDirection{ gradientDirection }
{
    
}

void UiColorGradient::AddColor(const GradientColorData colorData)
{
    m_gradientColors.push_back(colorData);
    std::sort(m_gradientColors.begin(), m_gradientColors.end(), [](const GradientColorData& dataPoint1, const GradientColorData& dataPoint2) {
        return dataPoint1.position < dataPoint1.position;
    });
    m_updateVertexData = true;
}

void UiColorGradient::ClearColorData()
{
    m_gradientColors.clear();
    m_updateVertexData = true;
}

void UiColorGradient::CreateResources()
{
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



    for (size_t i = 0; i < m_gradientColors.size(); ++i)
    {
        const auto colorData = m_gradientColors[i];

        if (colorData.position > 1.0f || colorData.position < 0.0f)
        {
            ULOGE("UiColorGradient Error: color data " << i << " out of bounce!");
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


    m_vertexBuffer = VertexBuffer::Create(sizeof(VertexDesc) * vertices.size(), reinterpret_cast<float*>(vertices.data()), GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(unsigned) * indices.size(), indices.data(), GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();
}

void UiColorGradient::HandleEvent(Event *event)
{
    UiBasicControl::HandleEvent(event);
}

void UiColorGradient::Render()
{
    m_shader->Bind();

    m_shader->SetFloat4x4("modelMatrix", m_perspective * GetTransform());

    m_layout->Bind();

    Application::GetRenderer().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());

    UiElement::Render();
}
