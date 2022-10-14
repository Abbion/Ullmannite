#pragma once
#include "Nodes/Node.h"
#include "Scene/SceneObjects/Camera.h"
#include <string>
#include "Event/EventHandler.h"
#include "Rendering/DrawInterfaces/Renderable.h"

namespace Ull
{
    class Scene : public EventHandler, public Renderable
    {
    public:
        Scene(std::string name);
        ~Scene();
        
        Node* GetNodeByName(const std::string& name) const;
        Node* GetRootNode() const { return m_root; }
        const Camera* GetMainCamera() const { return m_mainCamera; }
        inline const std::string GetName() const { return m_name; } 

        void SetMainCamera(Camera* camera) { m_mainCamera = camera; }

        void HandleEvent(Event* event) override;
        void Update();
        void Render() override;

    private:
        Node* m_root{ nullptr };
        Camera* m_mainCamera{ nullptr };
        std::string m_name;
    };
} 
