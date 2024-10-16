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

glm::mat4 Object2D::GetTransform() const
{
    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3{ m_position.x, m_position.y, 0.0f });
    transform = glm::scale(transform, glm::vec3{ m_size.x * m_scale, m_size.y * m_scale, 1.0f });
    return transform;
}
