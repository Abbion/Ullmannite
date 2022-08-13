#pragma once

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
        ~Renderer();

        static Renderer* GetInstance();

        void SetApi(API api);

        API inline GetApi() { return m_api; }

    protected:
        Renderer();
        static Renderer* m_rendererInstance{ nullptr };

    private:

        API m_api{ API::OPEN_GL };
    }
}