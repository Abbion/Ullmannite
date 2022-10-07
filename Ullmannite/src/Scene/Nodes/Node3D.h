#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "Node.h"
#include "RenderableNode.h"

namespace Ull
{
    class Scene;

    class Node3D : public RenderableNode
    {
    public:
        Node3D(const std::string& name, const Scene* scene);
        virtual ~Node3D() {}

        glm::vec3 GetPosition() const               { return m_position; }
        glm::vec3 GetScale() const                  { return m_scale; }
        glm::vec3 GetRotation() const               { return m_rotation; }
        glm::mat4x4 GetTransformationMatrix() const { return m_transformationMatrix; }

        glm::vec3 GetGlobalPosition() const;
        glm::vec3 GetGlobalScale() const;
        glm::vec3 GetGlobalRotation() const;

        void SetPosition(const glm::vec3 position)  { m_position = position; }
        void SetScale(const glm::vec3 scale)        { m_scale = scale; }
        void SetRotation(const glm::vec3 rotation)  { m_rotation = rotation; }

        void Translate(const glm::vec3 moveBy);
        void Scale(const glm::vec3 scaleBy);
        void Rotate(const glm::vec3 rotateBy);

        void CalculateTransformationMatrix();

    protected:
        glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
        glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f }; //TODO: Use quaterion

        glm::mat4x4 m_transformationMatrix;
    };
}