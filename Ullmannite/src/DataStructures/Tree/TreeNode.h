#pragma once
#include <optional>
#include <memory>
#include <string>
#include <vector>
#include "Utilities/PointerHelper.h"
#include "Utilities/NonCopyable.h"
#include "Logger/Logger.h"

namespace Ull
{
    namespace 
    {
        #ifdef GTEST
        constexpr auto DOWN_SYMBOL = L"| ";
        constexpr auto RIGHT_SYMBOL = L"> ";
        #else
        constexpr auto DOWN_SYMBOL = L"\u2502 ";
        constexpr auto RIGHT_SYMBOL = L"\u251C ";
        #endif
    }

    template<typename T>
    class TreeNode : private NonCopyable<TreeNode<T>>
    {
    public:
        typedef std::shared_ptr<T> TRef;

    public:
        virtual ~TreeNode()
        {
            for (auto& child : m_children)
            {
                if (child.use_count() > 1)
                    ULOGE("Element " << m_name << " was deleted but " << child->GetName() << " element is still in use");
            }

            m_children.clear();

            ULOGD("UiElement: " << m_name << " terminated!");
        }

        std::string         GetName() const     { return m_name; }
        std::size_t         GetId() const       { return m_id; }
        NotOwner<T>         GetParent() const   { return m_parent; }
        std::vector<TRef>&  GetChildren()       { return m_children; }
        NotOwner<T>         GetRoot() const     { return m_root; }
        std::optional<TRef> GetUiElement(const std::string& name)
        {
            const auto id = std::hash<std::string>{}(name);

            for (auto& child : m_children)
            {
                if (child->GetId() == id)
                    return child;
            }

            return std::nullopt;
        }

        void AddChildNode(TRef childNode)
        {
            const auto newId = childNode->GetId();
            bool idExists = false;

            if (m_root == nullptr)
            {
                if (newId == m_id)
                    idExists = true;
                else
                {
                    for (auto& child : m_children)
                    {
                        if (IdExists(newId, child.get()))
                            idExists = true;
                    }
                }
            }
            else
            {
                if (IdExists(newId, m_root.Get()))
                    idExists = true;
            }

            if (idExists)
            {
                ULOGE("Element: " << childNode->GetName() << " with ID: " << childNode->GetId() << " is present in: " << m_name);
                return;
            }

            m_children.push_back(childNode);
            childNode->SetParent(static_cast<T*>(this));
            childNode->UpdateRoot(m_root.Get());
        }

        void PrintTree(const uint32_t depth = 0)
        {
            std::wstring line;
            if (depth != 0)
            {
                for (uint32_t i = 0; i < depth - 1; ++i)
                    line.append(DOWN_SYMBOL);
                line.append(RIGHT_SYMBOL);
            }

            ULOGD(line << m_name);

            for (auto& child : m_children)
                child->PrintTree(depth + 1);
        }

    protected:
        TreeNode(const std::string& name) :
            m_name{ name },
            m_id{ std::hash<std::string>{}(m_name) }
        {}

        TreeNode(TreeNode&& source) noexcept :
            m_name{ source.m_name },
            m_id{ source.m_id },
            m_parent{ source.m_parent },
            m_children{ source.m_children }
        {}

    private:
        bool IdExists(const size_t id, T* uiElement) const
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

        void SetParent(NotOwner<T> parent) { m_parent = parent; }
        void UpdateRoot(T* root)
        {
            m_root = root;

            for (auto& child : m_children)
                child->UpdateRoot(root);
        }

        std::string m_name;
        std::size_t m_id{ 0 };

        NotOwner<T> m_parent{ nullptr };
        std::vector<TRef> m_children;
        NotOwner<T> m_root{ nullptr };
    };
}