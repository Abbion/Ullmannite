#include "Ullpch.h"
#include "CoordinateConverter.h"
#include "Logger/Logger.h"

using namespace Ull;

CoordinateConverter* CoordinateConverter::m_converterInstance = nullptr;

CoordinateConverter* CoordinateConverter::GetInstance()
{
    if(m_converterInstance == nullptr)
        m_converterInstance = new CoordinateConverter();

    return m_converterInstance;
}

CoordinateConverter::~CoordinateConverter()
{
    ULOGD("Coordinate converter terminated");
}

glm::vec2 CoordinateConverter::ScreenToNormal(glm::ivec2 position)
{
    glm::vec2 out;
    out.x = (((float)position.x / (float)m_windowSize.x) * 2.0f) - 1.0f;
    out.y = 1.0f - (((float)position.y / (float)m_windowSize.y) * 2.0f);
    return out;
}

void CoordinateConverter::SetWindowSize(glm::uvec2 size)
{
    //TODO: check if not zero???
    m_windowSize = size;
}