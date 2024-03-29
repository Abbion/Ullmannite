#include "Ullpch.h"
#include "Layer/Layer.h"
#include "Logger/Logger.h"

using namespace Ull;

Layer::Layer(const std::string& name, const glm::uvec2& size) : m_name(name)
{
    auto layoutName = m_name + " Layout";
    m_layout = new UiLayout(layoutName, glm::uvec2(0, 0), size);    
}

Layer::~Layer()
{
    delete m_layout;
    
    ULOGD("Layer " << m_name << " terminated");
}