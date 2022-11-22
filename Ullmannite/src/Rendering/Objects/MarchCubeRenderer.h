#pragma once
#include "DataStructures/VolumeData.h"
#include "Scene/Nodes/Node3D.h"
#include "Rendering/Api/Shader.h"
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Buffer.h"
#include "Utilities/PointerHelper.h"
#include "DataStructures/CuttingSettings.h"
#include <memory>

namespace Ull
{
    class MarchCubeRenderer : public Node3D
    {
    public:
        MarchCubeRenderer(const std::string& name, NotOwner<Scene> scene);
        ~MarchCubeRenderer();

        void SetVolumeData(const std::shared_ptr<VolumeData> volumeData);
        void SetTransferTexture(NotOwner<Texture1D> transferTexture);

        void GenerateMesh();

        void HandleEvent(Event* event) override;
        void Update() override;
        void Render() override;

    private:
        std::shared_ptr<VolumeData> m_volumeData{ nullptr };
        bool updateMesh{ false };

        glm::vec2 m_thresholds{ 0.0f, 0.0f };   //TODO: change to ivec2
        bool m_thresholdInitEventShip{ false };

        glm::ivec3 m_cuttingSettingsInt{ 0, 0, 0 };

        Shader* m_cubeMarchVertexCounter{ nullptr };
        Shader* m_cubeMarchShader{ nullptr };
        Shader* m_vertexRendererShader{ nullptr };

        NotOwner<Texture1D> m_transferTexture{ nullptr };
        Texture3D* m_volumeTexture{ nullptr };
        Texture3D* m_vertexPosTexture{ nullptr };
        VertexBuffer* m_vertexBuffer{ nullptr };
        uint64_t m_vertexCount{ 0 };

        uint64_t CalculateVertexCountGPU();
        void SetUpLight();

    };
}