#pragma once
#include "Window/Window.h"
#include "Ullpch.h"

namespace Ull
{
    class CoordinateConverter
    {
    public:
        NON_COPYABLE(CoordinateConverter)

        static CoordinateConverter* GetInstance();
        ~CoordinateConverter();

        glm::vec2 ScreenToNormal(glm::ivec2 position);

    protected:
        CoordinateConverter() {}
        static CoordinateConverter* m_converterInstance;
    
    private:
        void SetWindowSize(glm::uvec2 size);
        glm::uvec2 m_windowSize{1, 1};


        friend class Application;
    };
}