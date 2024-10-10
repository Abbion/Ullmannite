#pragma once
#include <optional>
#include <memory>
#include <string>
#include <vector>
#include "Utilities/PointerHelper.h"
#include "Utilities/NonCopyable.h"

namespace Ull
{
    class TreeNode;
    typedef std::shared_ptr<TreeNode> TreeNodeRef;

    class TreeNode : private NonCopyable<TreeNode>
    {
    public:
        TreeNode(const std::string& name);
        TreeNode(TreeNode&& source) noexcept;
        virtual ~TreeNode();

        std::string                     GetName() const     { return m_name; }
        std::size_t                     GetId() const       { return m_id; }
        NotOwner<TreeNode>              GetParent() const   { return m_parent; }
        std::vector<TreeNodeRef>&       GetChildren()       { return m_children; }
        NotOwner<TreeNode>              GetRoot() const     { return m_root; }
        std::optional<TreeNodeRef>      GetUiElement(const std::string& name);

        virtual void AddChildNode(TreeNodeRef childElement);
        void PrintTree(const uint32_t depth = 0);

    private:
        bool IdExists(const size_t id, TreeNode* uiElement) const;

        void SetParent(NotOwner<TreeNode> parent);
        void UpdateRoot(TreeNode* root);

        std::string m_name;
        std::size_t m_id{ 0 };

        NotOwner<TreeNode> m_parent{ nullptr };
        std::vector<TreeNodeRef> m_children;
        NotOwner<TreeNode> m_root{ nullptr };
    };
}