#pragma once
#include "Scene/Nodes/Node.h"

namespace Ull
{
    class DirectionalLight : public Node
    {
    public:
        DirectionalLight(const std::string& name, NotOwner<Scene> scene);

        void SetDirection(const glm::vec3& direction) { m_direction = direction; }
        void SetAmbientColor(const glm::vec3& color) { m_ambientColor = color; }
        void SetDiffuseColor(const glm::vec3& color) { m_diffuseColor = color; }

        glm::vec3 GetDirection() const { return m_direction; }
        glm::vec3 GetAmbientColor() const { return m_ambientColor; }
        glm::vec3 GetDiffuseColor() const { return m_diffuseColor; }

    private:
        glm::vec3 m_direction{ 0.0f, 0.0f, 1.0f };
        glm::vec3 m_diffuseColor{ 0.8f, 0.8f, 0.8f };
        glm::vec3 m_ambientColor{ 0.2f, 0.2f, 0.2f };
    };
}