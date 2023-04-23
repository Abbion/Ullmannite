#pragma once
#include "Ullpch.h"
#include "Buffer.h"
#include "Shader.h"
#include "ShaderManager.h"

#include <glad/glad.h>

namespace Ull
{
    class Renderer
    {
    public:
        enum class API : uint8_t
        {
            NONE = 0u,
            OPEN_GL = 1u
        };

        enum class ClearBits : uint8_t
        {
            COLOR = 1 << 0u,
            DEPTH = 1 << 1u,
            SETNCIL = 1 << 2u
        };

        enum class State
        {
            ENABLE,
            DISABLE
        };

        enum class FaceCulling
        {
            NONE,
            FRONT,
            BACK,
            FRONT_AND_BACK
        };

        enum class FaceWinding
        {
            CLOCKWISE,
            COUNTER_CLOCKWISE
        };

        enum class BarrierType
        {
            STORAGE_BUFFER_BARRIER,
            ATOMIC_COUNTER_BARRIER,
            IMAGE_BARRIER
        };

    public:
        NON_COPYABLE(Renderer);
        
        ~Renderer();

        static Renderer& GetInstance();

        inline API GetApi() { return m_api; }
        void SetApi(API api);
        void Init();
        void Terminate();

        ShaderManager& GetShaderManager() { return m_shaderManager; }

        void SetViewPort(const glm::uvec2& position, const glm::uvec2& size);

        void SetDepth(State state);
        void SetFaceCulling(FaceCulling culling);
        void SetFaceWinding(FaceWinding winding);

        void SetPixelPackWidth(unsigned int width);
        void SetPixelUnpackWidth(unsigned int width);

        void SetClearColor(glm::vec4 color);
        void Clear(ClearBits clearBits);
        void DrawElements(GraphicsRenderPrimitives primitive, unsigned int count, GraphicsDataType type = GraphicsDataType::UINT, unsigned int skip = 0);
        void DrawArrays(GraphicsRenderPrimitives primitive, unsigned int count, unsigned int skip = 0);
        void DispatchComputeShader(unsigned int groupSizeX, unsigned int groupSizeY, unsigned int groupSizeZ);
        void FlushContext();
        void Barrier(BarrierType barrier);

    protected:
        Renderer() = default;

        static Renderer m_rendererInstance;

    private:
        API m_api{ API::NONE };
        bool m_apiLocked{ false };

        ShaderManager m_shaderManager;
    };

    inline Renderer::ClearBits operator|(Renderer::ClearBits flagA, Renderer::ClearBits flagB)
    {
        return static_cast<Renderer::ClearBits>(static_cast<int>(flagA) | static_cast<int>(flagB));
    }

    inline bool operator&(Renderer::ClearBits flagA, Renderer::ClearBits flagB)
    {
        return static_cast<bool>(static_cast<int>(flagA) & static_cast<int>(flagB));
    }
}