#pragma once
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Shader.h"
#include <glm/glm.hpp>
#include <vector>

namespace Ull
{
    class TransferFunctionRenderer
    {
    public:
        TransferFunctionRenderer(const std::vector<GradientColorData>& transferPoints);
        ~TransferFunctionRenderer();

        void GenerateTransferFunction();
        Texture1D* GetTransferFunctionTexture() const { return m_transferFunctionTexture; }

    private:
        void Init();

        std::vector<GradientColorData> m_transferPoints;

        std::shared_ptr<Shader> m_transferFunctionGeneratorShader;
        Texture1D* m_transferFunctionTexture = nullptr;
    };
}