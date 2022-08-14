#pragma once
#include "Ullpch.h"
#include "Buffer.h"
#include "Shader.h"

namespace Ull
{
    class Renderer
    {
    public:
        enum class API
        {
            OPEN_GL = 0
        };

    public:
        NON_COPYABLE(Renderer);
        
        ~Renderer();

        static Renderer* GetInstance();

        void SetApi(API api);
        void init();

        API inline GetApi() { return m_api; }

    protected:
        Renderer() = default;
        inline static Renderer* m_rendererInstance{ nullptr };

    private:

        API m_api{ API::OPEN_GL };
    };
}