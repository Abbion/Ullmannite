#include "Ullpch.h"
#include "Layer/Layer.h"
#include "Logger/Logger.h"

using namespace Ull;

Layer::Layer(const std::string& name, const glm::uvec2& size) : 
    m_name{ name }
{
    if (size.x < 1 || size.y < 1)
        ULOGE("Layer Error: size cannot be 0. size x: " << size.x << " size.y: " << size.y);

    m_layout = std::make_shared<UiLayout>(m_name + " Layout", glm::uvec2(0, 0), size);
}

Layer::~Layer()
{
    ULOGD("Layer " << m_name << " terminated");
}