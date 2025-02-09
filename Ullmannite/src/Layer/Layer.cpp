#include "Ullpch.h"
#include "Layer/Layer.h"
#include "Logger/Logger.h"

using namespace Ull;

Layer::Layer(const std::string& name, const glm::uvec2 size)
{
    if (size.x < 1 || size.y < 1)
        ULOGE("Layer Error: size cannot be 0. size x: " << size.x << " size.y: " << size.y);

    m_layout = std::make_shared<UiLayout>(name + " LayerLayout", size);
}

void Layer::Update()
{
    for (auto& element : m_layout->GetChildren())
        element->Update();
}

void Layer::Render()
{
    if (m_layout->IsVisible())
        m_layout->Render();
}