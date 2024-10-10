#include "Ullpch.h"
#include "Object2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Event/EventHandler.h"

using namespace Ull;

Object2D::Object2D(const std::string& name, const glm::vec2 position, const glm::vec2 size) :
    TreeNode{ name },
    m_position{ position },
    m_size{ size }
{
    m_modelMatrix = glm::mat4x4(1.0f);
}

Object2D::Object2D(Object2D&& source) noexcept :
    TreeNode{ std::move(source) },
    m_position{ source.m_position },
    m_size{ source.m_size },
    m_modelMatrix{ source.m_modelMatrix },
    m_updateTransform{ source.m_updateTransform }
{
}

void Object2D::AddChildNode(TreeNodeRef childNode)
{
    if (dynamic_cast<Object2D*>(childNode.get()) == nullptr)
    {
        ULOGE(childNode->GetName() << " type is not Object2D");
        return;
    }

    TreeNode::AddChildNode(childNode);
}

glm::mat4x4 Object2D::GetTransform()
{
    if (m_updateTransform)
    {
        m_updateTransform = false;

        glm::vec2 position = m_position;
        auto parent = static_cast<Object2D*>(GetParent().Get());

        while (parent != nullptr)
        {
            position += parent->GetPosition();
            parent = static_cast<Object2D*>(parent->GetParent().Get());
        }

        m_modelMatrix = glm::mat4x4(1.0f);
        m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(position.x, position.y, 0.0f));
        m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_size.x, m_size.y, 0.0f));
    }

    return m_modelMatrix;
}

void Object2D::SetPosition(const glm::vec2 position)
{
    m_position = position;
    UpdateModelMatrix(this);
}

void Object2D::SetSize(const glm::vec2 size)
{
    m_size = size;
    UpdateModelMatrix(this);
}

void Object2D::SetScale(const float scale)
{
    m_scale = scale;
    UpdateModelMatrix(this);
}

void Object2D::UpdateModelMatrix(Object2D* object2D)
{
    object2D->m_updateTransform = true;

    for (auto& child : object2D->GetChildren())
        UpdateModelMatrix(static_cast<Object2D*>(child.get()));
}
