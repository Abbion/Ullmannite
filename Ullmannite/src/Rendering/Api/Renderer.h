#pragma once
#include "Ullpch.h"
#include "Buffer.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Ull
{
    class Renderer
    {
    public:
        enum class API : uint8_t
        {
            NONE = 0,
            OPEN_GL = 1
        };

        enum class ClearBits
        {
            COLOR = 1 << 0,
            DEPTH = 1 << 1,
            SETNCIL = 1 << 2
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

    public:
        NON_COPYABLE(Renderer);
        
        ~Renderer();

        static Renderer& GetInstance();

        void SetApi(API api);
        void Init();

        void SetViewPort(const glm::uvec2& position, const glm::uvec2& size);
        void SetClearColor(glm::vec4 color);
        void Clear(ClearBits clearBits);

        void SetDepth(State state);
        void SetFaceCulling(FaceCulling culling);
        void SetFaceWinding(FaceWinding winding);

        void DrawElements(GraphicsRenderPrimitives primitive, unsigned int count, GraphicsDataType type = GraphicsDataType::UINT, unsigned int skip = 0);
        void DrawArrays(GraphicsRenderPrimitives primitive, unsigned int count, unsigned int skip = 0);
        void FlushContext();

        inline API GetApi() { return m_api; }

    protected:
        Renderer() = default;
        static Renderer m_rendererInstance;

    private:
        API m_api{ API::NONE };
        bool m_apiLocked{ false };
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