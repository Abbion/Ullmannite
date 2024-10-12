#include "Ullpch.h"
#include "UIGradientEditor.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "glm/gtx/transform.hpp"
#include "Event/Event.h"
#include "Event/EventAggregator.h"
#include "Input/Mouse.h"
#include "Utilities/CollisionCheckers.h"
#include "Logger/Logger.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace Ull;

UiGradientEditor::UiGradientEditor(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiElement(name, position, size)
{
    auto& shaderManager = Renderer::GetInstance().GetShaderManager();
    m_shader = shaderManager.GetShader(ShaderTag::UI_GRADIENT_SHADER);
}

void UiGradientEditor::CreateResources()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    float vertices[] = { 
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = { 
        0, 1, 2,
        1, 3, 2 
    };

    m_layout = VertexLayout::Create({
        LayoutElement("Position", GraphicsDataType::FLOAT, 3),
        LayoutElement("TexCoords", GraphicsDataType::FLOAT, 1)
    });

    m_layout->Bind();

    m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();

    SetTransferFunction(m_transferFunctionRenderer); //TODO: DELETE THIS CANCER
}

void UiGradientEditor::SetTransferFunction(NotOwner<TransferFunctionRenderer> transferFunction)
{
    m_transferFunctionRenderer = transferFunction;
    auto points = m_transferFunctionRenderer->GetPoints();
    auto minMax = m_transferFunctionRenderer->GetMinMaxPos();
    auto pointCount = points.size();
    const auto position = GetPosition();
    const auto size = GetSize();

    m_markers.clear();
    m_markers.reserve(sizeof(GradientMarker) * pointCount);

    for(unsigned i = 0u; i < pointCount; ++i)
    {
        std::stringstream ss;
        ss << "Marker_" << i;

        auto transformMarkerPosition = (((float)points[i].position / (float)minMax.second) * (float)size.x);
        auto markerPosition = glm::uvec2(position.x - (size.x * 0.5f) + transformMarkerPosition, position.y + size.y * 0.6f);
        auto markerSize = glm::uvec2(size.y* 0.3f, size.y * 0.3f);
        auto markerColor = points[i].m_color;

        auto marker = GradientMarker(ss.str(), markerPosition, markerSize, glm::vec4(markerColor.x, markerColor.y, markerColor.z, 1.0f));
        marker.SetViewPos(m_viewPos);
        marker.SetViewSize(m_viewSize);
        marker.SetMinMaxBounds(static_cast<int>(position.x - (size.x / 2.0f) + 0.5f), static_cast<int>(position.x + (size.x / 2.0f) + 0.5f));
        marker.CreateResources();

        m_markers.push_back(std::move(marker));
    }
}

void UiGradientEditor::SetViewSize(glm::uvec2 size)
{
    m_viewSize = size;
    const auto sizeAAA = GetSize();

    for(auto& marker : m_markers)
        marker.SetViewSize(sizeAAA);
}

void UiGradientEditor::SetViewPos(glm::ivec2 pos)
{
    m_viewPos = pos;
    for(auto& marker : m_markers)
        marker.SetViewPos(m_viewPos);
}

void UiGradientEditor::HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::MouseDoubleUp:
        auto mousePos = Mouse::GetInstance().GetMousePosition();
        const auto position = GetPosition();
        const auto size = GetSize();

        if (PointInStaticRect<glm::ivec2>(mousePos - m_viewPos, glm::ivec2(position) - glm::ivec2(size) / 2, glm::ivec2(size)))
        {
            auto points = m_transferFunctionRenderer->GetPoints();
            auto minMax = m_transferFunctionRenderer->GetMinMaxPos();

            auto startPos = mousePos.x - ((int)position.x - (int)(size.x / 2));
            auto pos = unsigned(((float)(startPos) / (float)size.x) * minMax.second);

            auto transformMarkerPosition = (((float)pos / (float)minMax.second) * (float)size.x);
            auto markerPosition = glm::uvec2(position.x - (size.x * 0.5f) + transformMarkerPosition, position.y + size.y * 0.6f);
            auto markerSize = glm::uvec2(size.y * 0.3f, size.y * 0.3f);
            auto markerColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            auto marker = GradientMarker("Marker", markerPosition, markerSize, glm::vec4(markerColor.x, markerColor.y, markerColor.z, 1.0f));
            marker.SetViewPos(m_viewPos);
            marker.SetViewSize(m_viewSize);
            marker.SetMinMaxBounds(static_cast<int>(position.x - (size.x / 2.0f) + 0.5f), static_cast<int>(position.x + (size.x / 2.0f) + 0.5f));
            marker.CreateResources();

            m_markers.push_back(std::move(marker));
            m_transferFunctionRenderer->AddPoint({marker.GetColor(), (unsigned)(((float)(startPos) / (float)size.x) * 512)});
            m_transferFunctionRenderer->GenerateTransferFunction();
        }
    break;

    default:
        break;
    }

    for (auto& marker : m_markers)
    {
        marker.HandleEvent(event);
    }
}

void UiGradientEditor::Update()
{
    const auto position = GetPosition();
    const auto size = GetSize();

    auto recenterSize = glm::vec2(m_viewSize) / 2.0f;
    glm::vec2 translate = glm::vec2(-recenterSize.x + position.x, recenterSize.y - position.y) / recenterSize;

    //m_modelMatrix = glm::identity<glm::mat4x4>();
    //m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(translate.x, translate.y, 0.0f));
    //m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3((float)size.x / (float)m_viewSize.x, ((float)size.y * 0.5f) / (float)m_viewSize.y, 1.0f));

    //std::remove_if(m_markers.begin(), m_markers.end(), [](const GradientMarker& marker) { return marker.IsMarkerDeleted(); });

    std::vector<int> toDelete;
    bool refreshGradient = false;

    for (int i = 0; i < m_markers.size(); ++i)
    {
        if (m_markers[i].IsMarkerDeleted())
            toDelete.push_back(i);
    }

    for (auto itr : toDelete)
    {
        m_markers.erase(m_markers.begin() + itr);
        refreshGradient = true;
    }

    for (auto& marker : m_markers)
    {
        marker.Update();

        if(marker.IsMarkerActive())
            refreshGradient = true;
    }

    if(refreshGradient)
    {
        EventAggregator::Publish(std::make_shared<GradientUpdatedEvent>(EventType::GradientUpdated));

        m_transferFunctionRenderer->DeleteAppLoints();

        for (auto& marker : m_markers)
        {
            auto pos = (unsigned)(((float)(marker.GetPosition().x - (position.x - (size.x / 2))) / (float)size.x) *512);
            m_transferFunctionRenderer->AddPoint(TransferPoint{ marker.GetColor(),  pos});
        }

        m_transferFunctionRenderer->GenerateTransferFunction();
    }
}

void UiGradientEditor::Render()
{
    RenderGradient();

    for (auto& marker : m_markers)
        marker.Render();
}

void UiGradientEditor::RenderMarkers()
{

}

void UiGradientEditor::RenderGradient()
{
    m_layout->Bind();

    m_shader->Bind();

    m_shader->SetFloat4x4("modelMatrix", glm::mat4(1.0f));
    auto texture = m_transferFunctionRenderer->GetTransferFunctionTexture();
    texture->Bind();
    m_shader->SetInt("transferTexture", 0);

    Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
}

GradientMarker::GradientMarker(std::string name, glm::uvec2 position, glm::uvec2 size, glm::vec4 color) :
    UiElement(name, position, size),
    m_color(color),
    m_pointerColor(0.7f, 0.7f, 0.7f, 1.0f)
{
    auto& shaderManager = Renderer::GetInstance().GetShaderManager();
    m_shader = shaderManager.GetShader(ShaderTag::UI_BASIC_COLOR);
}

GradientMarker::GradientMarker(GradientMarker&& source) :
    UiElement(std::move(source)),
    m_markerGrabbed{ source .m_markerGrabbed },
    m_openColorMenu{ source.m_openColorMenu },
    m_colorMenuJustOpened{ source.m_colorMenuJustOpened },
    m_pickerOutClick{ source.m_pickerOutClick },
    m_color{ source.m_color },
    m_viewSize{ source.m_viewSize },
    m_viewPos{ source.m_viewPos },
    m_minMax{ source.m_minMax },
    m_pointerColor{ source.m_pointerColor }
{
}

GradientMarker::~GradientMarker()
{
    
}

void GradientMarker::CreateResources()
{
    if(m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if(m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    float vertices[] = { 
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f 
    };

    unsigned int indices[] = { 
        0, 1, 2,
        1, 3, 2,
        2, 3, 4 
    };

    m_layout = VertexLayout::Create({
        LayoutElement("Position", GraphicsDataType::FLOAT, 3)
    });

    m_layout->Bind();

    m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();
}

void GradientMarker::HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::MouseDown:
    {
        auto mouseEvent = static_cast<MouseDownEvent*>(event);

        if (mouseEvent->GetVal() == Mouse::Button::LEFT)
        {
            auto mousePos = Mouse::GetInstance().GetMousePosition();
            if (PointInMarker(mousePos))
            {
                if(!m_openColorMenu)
                    m_markerGrabbed = true;
            }

            m_pickerOutClick = true;
        }
        else if (mouseEvent->GetVal() == Mouse::Button::RIGHT)
        {
            m_pickerOutClick = true;
        }
    }
    break;

    case EventType::MouseUp:
    {
        auto mouseEvent = static_cast<MouseUpEvent*>(event);

        if (mouseEvent->GetVal() == Mouse::Button::LEFT)
        {
            m_markerGrabbed = false;
        }
        else if (mouseEvent->GetVal() == Mouse::Button::RIGHT)
        {
            auto mousePos = Mouse::GetInstance().GetMousePosition();

            if (PointInMarker(mousePos))
            {
                m_openColorMenu = true;
                m_colorMenuJustOpened = true;
            }
        }
    }
    break;

    }
}
void GradientMarker::Update()
{
    /*if (m_markerGrabbed)
    {
        auto mousePosOnView = Mouse::GetInstance().GetMousePosition() - m_viewPos;
        m_position.x = mousePosOnView.x;

        if (m_position.x < (unsigned)m_minMax.x)
            m_position.x = m_minMax.x;
        else if(m_position.x > (unsigned)m_minMax.y)
            m_position.x = m_minMax.y;
    }

    if (PointInMarker(Mouse::GetInstance().GetMousePosition()))
    {
        m_pointerColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        m_hovered = true;
    }
    else
    {
        m_pointerColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        m_hovered = false;
    }

    m_modelMatrix = glm::identity<glm::mat4x4>();
    
    auto recenterSize = glm::vec2(m_viewSize) / 2.0f;
    glm::vec2 translate = glm::vec2(-recenterSize.x + m_position.x, recenterSize.y - m_position.y) / recenterSize;

    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(translate.x, translate.y, 0.0f));

    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3((float)m_size.x / (float)m_viewSize.x, (float)m_size.y / (float)m_viewSize.y, 1.0f));*/
}
void GradientMarker::Render()
{
 /*   m_layout->Bind();

    m_shader->Bind();

    m_shader->SetFloat4("color", m_pointerColor);
    m_shader->SetFloat4x4("modelMatrix", m_modelMatrix);

    Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());

    glm::mat4x4 innerColorMatrix = m_modelMatrix;
    innerColorMatrix = glm::scale(innerColorMatrix, glm::vec3(0.85f, 0.85f, 0.85f));
    
    m_shader->SetFloat4("color", m_color);
    m_shader->SetFloat4x4("modelMatrix", innerColorMatrix);

    Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize() - 2);

    if (m_openColorMenu)
    {
        ImVec4 pickerColor = ImVec4(m_color.x, m_color.y, m_color.z, 1.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 12));

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

        ImGui::OpenPopup("colorPickPopup");
        if (ImGui::BeginPopup("colorPickPopup"))
        {
            ImGui::BeginGroup();
            ImGui::Text("Color picker");
            ImGui::ColorPicker3("##MyColor##6", (float*)&pickerColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
            if (ImGui::Button("Delete marker"))
            {
                m_markerDeleted = true;
            }
            ImGui::EndGroup();
            if (m_pickerOutClick && !m_colorMenuJustOpened && !ImGui::IsItemHovered())
            {
                m_openColorMenu = false;
            }

            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        m_color = glm::vec4(pickerColor.x, pickerColor.y, pickerColor.z, 1.0f);

        m_pickerOutClick = false;
        m_colorMenuJustOpened = false;
    }*/
}

inline bool GradientMarker::PointInMarker(glm::ivec2 point)
{
    const auto position = GetPosition();
    const auto size = GetSize();
    return PointInStaticRect<glm::ivec2>(point - m_viewPos, glm::ivec2(position) - glm::ivec2(size) / 2, glm::ivec2(size));
}