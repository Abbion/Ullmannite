#include "Ullpch.h"
#include "UiView3D.h"
#include "Rendering/Shapes/Cube.h"
#include <string>

using namespace Ull;

UiView3D::UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiArea(name, position, size),
    m_scene("Scene 3D")
{
    SetBackgroundColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));

    Init();
}

void UiView3D::Init()
{
    auto root = m_scene.GetRootNode();
    
    root->AddNode(new Cube());
}

void UiView3D:: HandleEvent(Event* event)
{
    
}

void UiView3D::Update()
{

}

void UiView3D::Render()
{
    if(m_areaUpdated)
    {
        m_frameBuffer->Bind();

		RenderBackground();
        m_scene.Render();

		m_frameBuffer->Unbind();

		m_areaUpdated = false;
    }
}