#pragma once

namespace Ull
{
    class Object2D
    {
    public:
        Object2D(const glm::vec2 position, const glm::vec2 size);
        Object2D(Object2D&& source) noexcept;

        glm::vec2 GetPosition() const       { return m_position; }
        glm::vec2 GetSize() const           { return m_size; }
        float GetScale() const              { return m_scale; }

        void SetPosition(const glm::vec2 position);
        void SetSize(const glm::vec2 size);
        void SetScale(const float scale);

    private:
        glm::vec2 m_position{ 0.0f, 0.0f };
        glm::vec2 m_size{ 1.0f, 1.0f };
        float m_scale{ 1.0f };
    };
}
