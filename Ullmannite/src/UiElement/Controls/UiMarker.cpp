#include "Ullpch.h"
#include "UiMarker.h"
#include "Application/Application.h"

namespace
{
    constexpr auto MARKER_COLOR_OUTLINE_THICKNESS = 1.2f;
}

namespace Ull
{
    UiMarker::UiMarker(const std::string name, const glm::vec2 position, const glm::vec2 size) :
        UiBasicControl(name, position, size, UiControlType::UiMarker),
        m_markerColor{ std::make_shared<UiSpace>(name + "Color", glm::vec2(0.0f, 0.0f), size) },
        m_markerColorOutline{ std::make_shared<UiSpace>(name + "ColorOutline", glm::vec2(0.0f, 0.0f), size) }
    {
        m_dragBounderies.x = position.x;
        m_dragBounderies.y = position.x;

        CreateControls();
    }

    float UiMarker::GetPositionRatio() const
    {
        const float bounderiesDistance = m_dragBounderies.y - m_dragBounderies.x;
        return (GetPosition().x - m_dragBounderies.x) / bounderiesDistance;
    }

    void UiMarker::CreateResources()
    {
        if (m_vertexBuffer != nullptr)
            delete m_vertexBuffer;

        if (m_indexBuffer != nullptr)
            delete m_indexBuffer;

        if (m_layout != nullptr)
            delete m_layout;

        float vertices[] = {
            0.0f, 0.3f, 0.0f,
            1.0f, 0.3f, 0.0f,
            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.5f, 0.0f, 0.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            1, 3, 2,
            0, 1, 4
        };

        m_layout = VertexLayout::Create({
            LayoutElement("Vertices", GraphicsDataType::FLOAT, 3)
            });

        m_layout->Bind();

        m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
        m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

        m_layout->Build();
        m_vertexBuffer->Unbind();
        m_layout->Unbind();
    }

    void UiMarker::SetMarkerColor(const glm::vec4 color)
    {
        m_markerColor->SetBackgroundColor(color);
    }

    void UiMarker::SetDragBounderies(const float left, const float right)
    {
        if (left > right)
        {
            ULOGE("Ui marker " << GetName() << " bounderies are invalid left > right");
            return;
        }

        m_dragBounderies.x = left;
        m_dragBounderies.y = right;
    }

    void UiMarker::HandleEvent(Event* event)
    {
        if (event->IsHandeled())
            return;

        switch (event->GetType())
        {
        case EventType::MouseMove:
        {
            if (IsVisible() == false)
                return;

            if (m_grabbed)
            {
                const auto currentMousePositon = Application::GetMouse().GetMousePosition();
                const auto direction = currentMousePositon - (glm::ivec2(GetPosition()) + m_grabPoint);

                auto newPosition = GetPosition() + glm::vec2(direction.x, 0.0f);
                newPosition.x = std::clamp(newPosition.x, m_dragBounderies.x, m_dragBounderies.y);
                SetPosition(newPosition);
                event->MarkHandeled(true);
            }
            break;
        }

        case EventType::MouseDown:
            if (IsVisible() == false)
                return;

            CheckHover();
            if (m_hover)
            {
                m_grabbed = true;
                m_grabPoint = Application::GetMouse().GetMousePosition() - glm::ivec2(GetPosition());
                event->MarkHandeled(true);
            }
            break;

        case EventType::MouseUp:
            if (IsVisible() == false)
                return;

            CheckHover();
            m_grabbed = false;
            break;

        case EventType::MouseDoubleUp:
            if (IsVisible() == false)
                return;

            if (m_hover)
            {
                const auto colorPickerData = ColorPickerData{ m_markerColor->GetBackgroundColor(), 
                                                              std::bind(&UiMarker::SetMarkerColor, this, std::placeholders::_1) };
                Application::GetEventQueue().PushEvent(std::make_shared<OpenToolEvent>(
                    EventType::OpenTool, ToolSetup{ ToolType::ColorPicker, glm::uvec2(50, 25), colorPickerData }));

                m_grabbed = false;
                event->MarkHandeled(true);
            }
            break;

        default:
            break;
        }

        UiBasicControl::HandleEvent(event);
    }

    void UiMarker::CreateControls()
    {
        const auto markerSize = GetSize();
        const auto minSize = min(markerSize.x, markerSize.y);
        const auto markerColorSize = glm::vec2(minSize) * 0.7f;

        const auto markerColorPosition = glm::vec2((markerSize.x / 2.0f) - (markerColorSize.x / 2.0f), (markerSize.y * 0.66f) - (markerColorSize.y / 2.0f));

        const auto markerColorOutlineThicknessSize = markerColorSize * MARKER_COLOR_OUTLINE_THICKNESS;
        const auto markerColorOutlineThicknessOffset = markerColorOutlineThicknessSize - markerColorSize;

        m_markerColorOutline->SetSize(markerColorOutlineThicknessSize);
        m_markerColorOutline->SetPosition(markerColorPosition - glm::vec2(markerColorOutlineThicknessOffset.x / 2.0f, markerColorOutlineThicknessOffset.y / 2.0f));
        m_markerColorOutline->CreateResources();
        m_markerColorOutline->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
        m_markerColorOutline->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        AddChildNode(m_markerColorOutline);

        m_markerColor->SetSize(markerColorSize);
        m_markerColor->SetPosition(markerColorPosition);
        m_markerColor->CreateResources();
        m_markerColor->SetFunctionality(UiControlFunctionality::Hover, State::Disable);

        AddChildNode(m_markerColor);
    }
}