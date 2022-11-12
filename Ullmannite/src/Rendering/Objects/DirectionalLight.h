#pragma once
#include "Scene/Nodes/Node.h"

namespace Ull
{
    class DirectionalLight : public Node
    {
    public:
        DirectionalLight(const std::string& name, NotOwner<Scene> scene);

        void SetDirection(const glm::vec3& direction) { m_direction = direction; }
        void SetLightColor(const glm::vec3& color) { m_lightColor = color; }
        void SetAmbientStrength(const float strength) { m_ambientStrength = strength; }

        glm::vec3 GetDirection() const { return m_direction; }
        glm::vec3 GetLightColor() const { return m_lightColor; }
        float GetAmbientStrength() const { return m_ambientStrength; }

    private:
        glm::vec3 m_direction{ 0.0f, 0.0f, 1.0f };
        glm::vec3 m_lightColor{ 0.8f, 0.8f, 0.8f };
        float m_ambientStrength{ 0.1f };
    };
}