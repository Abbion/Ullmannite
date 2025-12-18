#include "Ullpch.h"
#include "UiFrame.h"

using namespace Ull;

UiFrame::UiFrame(const std::string name, const glm::vec2 position, const glm::vec2 size) :
	UiBasicControl(name, position, size, UiControlType::UiFrame)
{
}

void UiFrame::CreateResources()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    const auto size = GetSize();
    const auto maxHorizontalThickness = std::abs(size.x / 2.0f);
    const auto maxVerticalThickness = std::abs(size.y / 2.0f);

    const auto horizontalThickness = m_thickness > maxHorizontalThickness ? maxHorizontalThickness : m_thickness;
    const auto verticalThickness = m_thickness > maxVerticalThickness ? maxVerticalThickness : m_thickness;

    const auto horizontalVertexOffset = (horizontalThickness * 0.5f) / maxHorizontalThickness;
    const auto verticalVertexOffset = (verticalThickness * 0.5f) / maxVerticalThickness;

    std::vector<glm::vec3> vertices;
    vertices.reserve(8);

    vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
    vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f));
    vertices.emplace_back(glm::vec3(horizontalVertexOffset, verticalVertexOffset, 0.0f));
    vertices.emplace_back(glm::vec3(1.0f - horizontalVertexOffset, verticalVertexOffset, 0.0f));
    vertices.emplace_back(glm::vec3(1.0f, 1.0f, 0.0f));
    vertices.emplace_back(glm::vec3(1.0f - horizontalVertexOffset, 1.0f - verticalVertexOffset, 0.0f));
    vertices.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));
    vertices.emplace_back(glm::vec3(horizontalVertexOffset, 1.0f - verticalVertexOffset, 0.0f));

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3,

        1, 4, 3,
        3, 4, 5,

        7, 5, 6,
        6, 5, 4,

        0, 2, 7,
        0, 7, 6
    };

    m_layout = VertexLayout::Create({
        LayoutElement("Vertices", GraphicsDataType::FLOAT, 3)
        });

    m_layout->Bind();

    m_vertexBuffer = VertexBuffer::Create(sizeof(glm::vec3) * vertices.size(), reinterpret_cast<float*>(vertices.data()), GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();
}
