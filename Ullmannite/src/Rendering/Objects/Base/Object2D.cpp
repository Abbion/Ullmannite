#include "Ullpch.h"
#include "Object2D.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

Object2D::Object2D(const glm::vec2 position, const glm::vec2 size) :
    m_position{ position },
    m_size{ size }
{
}

Object2D::Object2D(Object2D&& source) noexcept :
    m_position{ source.m_position },
    m_size{ source.m_size }
{
}

void Object2D::SetPosition(const glm::vec2 position)
{
    m_position = position;
}

void Object2D::SetSize(const glm::vec2 size)
{
    m_size = size;
}

void Object2D::SetScale(const float scale)
{
    m_scale = scale;
}