#pragma once
#include "glm/glm.hpp"
#include <DataStructures/Tree/TreeNode.h>

namespace Ull
{
    class Object2D;
    typedef std::shared_ptr<Object2D> Object2DRef;

    class Object2D : public TreeNode
    {
    public:
        Object2D(const std::string& name, const glm::vec2 position, const glm::vec2 size);
        Object2D(Object2D&& source) noexcept;

        virtual void AddChildNode(TreeNodeRef childNode);

        glm::vec2 GetPosition() const       { return m_position; }
        glm::vec2 GetSize() const           { return m_size; }
        float GetScale() const              { return m_scale; }
        glm::mat4x4 GetTransform();

        void SetPosition(const glm::vec2 position);
        void SetSize(const glm::vec2 size);
        void SetScale(const float scale);

    private:
        glm::vec2 m_position{ 0.0f, 0.0f };
        glm::vec2 m_size{ 1.0f, 1.0f };
        float m_scale{ 1.0f };
        bool m_updateTransform{ true };

        glm::mat4x4 m_modelMatrix;

        void UpdateModelMatrix(Object2D* object2D);
    };
}
