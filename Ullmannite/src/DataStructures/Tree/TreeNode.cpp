#include "Ullpch.h"
#include "TreeNode.h"
#include "Logger/Logger.h"

using namespace Ull;

TreeNode::TreeNode(const std::string& name) :
    m_name{ name },
    m_id{ std::hash<std::string>{}(m_name) }
{
}

TreeNode::TreeNode(TreeNode&& source) noexcept :
    m_name{ source.m_name },
    m_id{ source.m_id },
    m_parent{ source.m_parent },
    m_children{ source.m_children }
{
}

TreeNode::~TreeNode()
{
    for (auto& child : m_children)
    {
        if (child.use_count() > 1)
            ULOGE("Element " << m_name << " was deleted but " << child->GetName() << " element is still in use");
    }

    m_children.clear();

    ULOGD("UiElement: " << m_name << " terminated!");
}

bool TreeNode::IdExists(const size_t id, TreeNode* uiElement) const
{
    if (m_id == id)
        return true;

    for (const auto& child : uiElement->GetChildren())
    {
        if (IdExists(id, child.get()))
            return true;
    }

    return false;
}

void TreeNode::UpdateRoot(TreeNode* root)
{
    m_root = root;

    for (auto& child : m_children)
        child->UpdateRoot(root);
}

void TreeNode::AddChildNode(TreeNodeRef childNode)
{
    const auto newId = childNode->GetId();

    if (IdExists(newId, m_root.Get() == nullptr ? this : m_root.Get()))
    {
        ULOGE("Element: " << childNode->GetName() << " with ID: " << childNode->GetId() << " is present in: " << m_name);
        return;
    }

    m_children.push_back(childNode);
    childNode->SetParent(this);
    childNode->UpdateRoot(m_root.Get());
}

void TreeNode::SetParent(NotOwner<TreeNode> parent)
{
    m_parent = parent;
}

std::optional<TreeNodeRef> TreeNode::GetUiElement(const std::string& name)
{
    const auto id = std::hash<std::string>{}(name);

    for (auto& child : m_children)
    {
        if (child->GetId() == id)
            return child;
    }

    return std::nullopt;
}

void TreeNode::PrintTree(const uint32_t depth)
{
    std::wstring line;
    if (depth != 0)
    {
        for (uint32_t i = 0; i < depth - 1; ++i)
            line.append(L"\u2502 ");
        line.append(L"\u251C ");
    }

    ULOGD(line << m_name);

    for (auto& child : m_children)
        child->PrintTree(depth + 1);
}