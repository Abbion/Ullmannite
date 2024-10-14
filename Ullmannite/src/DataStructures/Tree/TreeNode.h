#pragma once
#include <optional>
#include <memory>
#include <string>
#include <vector>
#include "Utilities/PointerHelper.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/StrongType.h"
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

    typedef StrongType<size_t, struct NodeIdStruct, StrongTypeEq> NodeId;

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
        NodeId              GetId() const       { return m_id; }
        NotOwner<T>         GetParent() const   { return m_parent; }
        std::vector<TRef>&  GetChildren()       { return m_children; }
        NotOwner<T>         GetRoot()
        {
            T* currentRoot = m_self.Get();
            auto parent = GetParent().Get();

            while (parent != nullptr)
            {
                currentRoot = parent;
                parent = parent->GetParent().Get();
            }

            return currentRoot;
        }

        std::optional<NotOwner<T>> GetNodeByName(const std::string& name)
        {
            const auto id = NodeId(std::hash<std::string>{}(name));
            auto result = GetNodeByName(m_self.Get(), id);

            if (result)
                return NotOwner(result.value());
            return std::nullopt;
        }

        void AddChildNode(TRef childNode)
        {
            std::vector<NodeId> idVector;
            idVector.reserve(GetTotalSize(childNode.get()));
            GetAllIdsInBranch(childNode.get(), idVector);

            bool idExists = false;
            auto rootRef = GetRoot().Get();

            for (const auto id : idVector)
            {
                if (IdExists(rootRef, id))
                {
                    idExists = true;
                    break;
                }
            }

            if (idExists)
            {
                #ifndef GTEST
                ULOGE("Element: " << childNode->GetName() << " with ID: " << childNode->GetId() << " is present in: " << m_name);
                #endif 
                return;
            }

            m_children.push_back(childNode);
            childNode->SetParent(static_cast<T*>(this));
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
            m_id{ std::hash<std::string>{}(m_name) },
            m_self{ static_cast<T*>(this) }
        {}

        TreeNode(TreeNode&& source) noexcept :
            m_name{ source.m_name },
            m_id{ source.m_id },
            m_parent{ source.m_parent },
            m_children{ source.m_children },
            m_self{ source.m_self }
        {}

    private:
        bool IdExists(T* node, const NodeId id) const
        {
            if (node->GetId() == id)
                return true;

            for (const auto& child : node->GetChildren())
            {
                if (IdExists(child.get(), id))
                    return true;
            }

            return false;
        }

        std::optional<T*> GetNodeByName(T* node, const NodeId id)
        {
            if (node->GetId() == id)
                return node;

            for (auto& child : node->GetChildren())
            {
                auto result = GetNodeByName(child.get(), id);
                if (result)
                    return result;
            }

            return std::nullopt;
        }

        void GetAllIdsInBranch(T* node, std::vector<NodeId>& idVector)
        {
            idVector.emplace_back(node->GetId());

            for (auto& child : node->GetChildren())
                GetAllIdsInBranch(child.get(), idVector);
        }

        uint64_t GetTotalSize(T* node, uint64_t currentSize = 0)
        {
            currentSize++;

            for (auto& child : node->GetChildren())
                currentSize += GetTotalSize(child.get(), currentSize);

            return currentSize;
        }

        void SetParent(NotOwner<T> parent) { m_parent = parent; }

        const std::string m_name;
        const NodeId m_id;

        const NotOwner<T> m_self;
        NotOwner<T> m_parent{ nullptr };
        std::vector<TRef> m_children;
    };
}