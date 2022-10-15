#pragma once
#include "Nodes/Node.h"
#include "Scene/SceneObjects/Camera.h"
#include <string>
#include "Event/EventHandler.h"
#include "Rendering/DrawInterfaces/Renderable.h"
#include "Utilities/PointerHelper.h"

namespace Ull
{
    class Scene : public EventHandler, public Renderable
    {
    public:
        Scene(std::string name);
        ~Scene();
        
        bool IsUpdated() const { return m_isUpdated; }
        void SetUpdated(bool updated) const { m_isUpdated = updated; }
        void SetMainCamera(NotOwner<Camera> camera) { m_mainCamera = camera; }

        Node* GetNodeByName(const std::string& name) const;
        Node* GetRootNode() const { return m_root; }
        NotOwner<Camera> GetMainCamera() const { return m_mainCamera; }
        inline const std::string GetName() const { return m_name; } 

        void HandleEvent(Event* event) override;
        void Update();
        void Render() override;

    private:
        Node* m_root{ nullptr };
        NotOwner<Camera> m_mainCamera{ nullptr };
        std::string m_name;
        mutable bool m_isUpdated{ false };
    };
} 
