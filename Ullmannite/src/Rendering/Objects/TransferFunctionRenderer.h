#pragma once
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Shader.h"
#include <glm/glm.hpp>
#include <vector>

namespace Ull
{
    struct TransferPoint
    {
        glm::vec3 m_color;
        uint32_t position;
    };

    class TransferFunctionRenderer
    {
    public:
        TransferFunctionRenderer();
        TransferFunctionRenderer(const std::vector<TransferPoint>& transferPoints);
        ~TransferFunctionRenderer();

        std::vector<TransferPoint>& GetPoints() { return m_transferPoints; }
        
        void AddPoint(const TransferPoint& point);
        void EditPoint(const TransferPoint& point);
        void DeletePoint(uint16_t position);
        void DeleteAppLoints();

        void GenerateTransferFunction();
        Texture1D* GetTransferFunctionTexture() const { return m_transferFunctionTexture; }


        constexpr std::pair<uint32_t, uint32_t> GetMinMaxPos() { return std::make_pair(0u, 512u); } //TODO: normalize this to 0 - 1
    private:
        void Init();

        std::vector<TransferPoint> m_transferPoints;

        std::shared_ptr<Shader> m_transferFunctionGeneratorShader;
        Texture1D* m_transferFunctionTexture;
    };
}