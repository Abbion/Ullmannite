#pragma once
#include "Scene/Nodes/Node3D.h"
#include "Rendering/Api/Shader.h"
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Buffer.h"
#include "Utilities/PointerHelper.h"
#include <memory>

namespace Ull
{
    class MarchCubeRenderer : public Node3D
    {
    public:
        MarchCubeRenderer(const std::string& name, NotOwner<Scene> scene);
        ~MarchCubeRenderer();

        void SetTransferFunction(Texture1D* transferTexture) { m_transferTexture = transferTexture; }
        void SetThresholdValues(glm::uint min, glm::uint max);

        void HandleEvent(Event* event) override;
        void Update() override;
        void Render() override;

    private:
        void GenerateMesh();
        void CreateVolumeResources();

        bool updateMesh{ false };

        glm::uvec2 m_thresholds{ 0, 0 };
        bool m_thresholdInitEventShip{ false };

        glm::ivec3 m_cuttingSettingsInt{ 0, 0, 0 };

        std::shared_ptr<Shader> m_cubeMarchVertexCounter{ nullptr };
        std::shared_ptr<Shader> m_cubeMarchShader{ nullptr };
        std::shared_ptr<Shader> m_vertexRendererShader{ nullptr };

        Ull::NotOwner<Texture1D> m_transferTexture{ nullptr };
        Texture3D* m_volumeTexture{ nullptr };
        Texture3D* m_vertexPosTexture{ nullptr };
        VertexBuffer* m_vertexBuffer{ nullptr };
        uint64_t m_vertexCount{ 0 };

        uint64_t CalculateVertexCountGPU();
        void SetUpLight();

    };
}