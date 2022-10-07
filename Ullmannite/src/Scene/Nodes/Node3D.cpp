#include "Ullpch.h"
#include "Node3D.h"
#include "Scene/Scene.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

Node3D::Node3D(const std::string& name, const Scene* scene) : 
    RenderableNode(name, scene)
{
    m_transformationMatrix = glm::mat4x4(1.0f);
}

glm::vec3 Node3D::GetGlobalPosition() const
{
    auto parent = GetParent();
    auto globalPosition = m_position;

    while(parent)
    {
        globalPosition += static_cast<const Node3D*>(parent)->GetPosition();
        parent = parent->GetParent();
    }

    return globalPosition;
}

glm::vec3 Node3D::GetGlobalScale() const
{
    auto parent = GetParent();
    auto globalScale = m_scale;

    while(parent)
    {
        globalScale += static_cast<const Node3D*>(parent)->GetScale();
        parent = parent->GetParent();
    }

    return globalScale;
}

glm::vec3 Node3D::GetGlobalRotation() const
{
    auto parent = GetParent();
    auto globalRotation = m_scale;

    while(parent)
    {
        globalRotation += static_cast<const Node3D*>(parent)->GetRotation();
        parent = parent->GetParent();
    }

    return globalRotation;
}

void Node3D::Translate(const glm::vec3 moveBy)
{
    m_position += moveBy;
}

void Node3D::Scale(const glm::vec3 scaleBy)
{
    m_scale += scaleBy;
}


void Node3D::Rotate(const glm::vec3 rotateBy)
{
    m_rotation += rotateBy;
}

void Node3D::CalculateTransformationMatrix()
{
    m_transformationMatrix = glm::mat4(1.0f);
    //m_transformationMatrix = glm::rotate(m_transformationMatrix, )
    m_transformationMatrix = glm::scale(m_transformationMatrix, m_scale);
    m_transformationMatrix = glm::translate(m_transformationMatrix, m_position);
}