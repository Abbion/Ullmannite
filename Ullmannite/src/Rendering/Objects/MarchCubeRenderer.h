#pragma once
#include "DataStructures/VolumeData.h"
#include "Scene/Nodes/Node3D.h"
#include "Rendering/Api/Shader.h"
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Buffer.h"
#include <memory>

namespace Ull
{
    class MarchCubeRenderer : public Node3D
    {
    public:
        MarchCubeRenderer(const std::string& name, NotOwner<Scene> scene);
        ~MarchCubeRenderer();

        void SetVolumeData(const std::shared_ptr<VolumeData> volumeData);
        void SetTransferexture();
        void SetMinSampling(uint16_t sampling) { m_minSampling = sampling; }
        void SetMaxSampling(uint16_t sampling) { m_maxSampling = sampling; }

        void GenerateMesh();

        void HandleEvent(Event* event) override;
        void Update() override;
        void Render() override;

    private:
        std::shared_ptr<VolumeData> m_volumeData{ nullptr };
        bool updateMesh{ false };

        uint16_t m_minSampling{ 10 };
        uint16_t m_maxSampling{ 5000 };

        std::vector<glm::vec3> m_mesh;

        Shader* m_cubeMarchVertexCounter{ nullptr };
        Shader* m_cubeMarchShader{ nullptr };
        Shader* m_vertexRendererShader{ nullptr };

        Texture3D* m_volumeTexture{ nullptr };
        Texture3D* m_vertexPosTexture{ nullptr };
        VertexBuffer* m_vertexBuffer{ nullptr };
        uint64_t m_vertexCount{ 0 };

        uint64_t CalculateVertexCountGPU();
        uint64_t CalculateVertexCountCPU();
    };
}