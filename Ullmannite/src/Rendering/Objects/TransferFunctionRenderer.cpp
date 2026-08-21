#include "Ullpch.h"
#include "TransferFunctionRenderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "Application/Application.h"
#include "Rendering/Api/Buffer.h"
#include <algorithm>

using namespace Ull;

namespace 
{
    constexpr float TRANSFER_FUNCTION_TEXTURE_WIDTH_F = 512.0f;
    constexpr uint16_t TRANSFER_FUNCTION_TEXTURE_WIDTH_UI = 512;
}

TransferFunctionRenderer::TransferFunctionRenderer(const std::vector<GradientColorData>& transferPoints) :
    m_transferPoints(transferPoints)
{
    Init();
}

TransferFunctionRenderer::~TransferFunctionRenderer()
{
    if(m_transferFunctionTexture != nullptr)
        delete m_transferFunctionTexture;
}

void TransferFunctionRenderer::GenerateTransferFunction()
{
    if (m_transferPoints.empty())
        return;

    std::sort(m_transferPoints.begin(), m_transferPoints.end(), [](const GradientColorData& a, const GradientColorData& b){ return a.position < b.position; });

    auto size = m_transferPoints.size();

    struct GradientColorDataGPUAligned {
        glm::vec3 color;
        float position;
    };

    std::vector<GradientColorDataGPUAligned> gpuAlignedTransferPoints;

    for (const auto transferPoint : m_transferPoints)
    {
        gpuAlignedTransferPoints.push_back(GradientColorDataGPUAligned{ glm::vec3(transferPoint.color.r, transferPoint.color.g, transferPoint.color.b), transferPoint.position });
    }

    StorageBuffer* transferPointsBuffer = StorageBuffer::Create(gpuAlignedTransferPoints.data(), sizeof(GradientColorDataGPUAligned) * size);

    m_transferFunctionTexture->BindImage(InternalDataFormat::RGBA_32F, ReadWriteRights::READ, 0);
    transferPointsBuffer->Bind(1);

    m_transferFunctionGeneratorShader->Bind();
    m_transferFunctionGeneratorShader->SetUint("colorPointsCount", (unsigned int)size);
    m_transferFunctionGeneratorShader->SetFloat("textureWidth", TRANSFER_FUNCTION_TEXTURE_WIDTH_F);

    Application::GetRenderer().DispatchComputeShader(1, 1, 1);
    Application::GetRenderer().Barrier(Renderer::BarrierType::IMAGE_BARRIER);
    
    m_transferFunctionTexture->Unbind();

    delete transferPointsBuffer;
}

void TransferFunctionRenderer::Init()
{
    auto& shaderManager = Application::GetRenderer().GetShaderManager();
    m_transferFunctionGeneratorShader = shaderManager.GetShader(ShaderTag::TRANSFER_FUNCTION_GENERATOR);
    
    m_transferFunctionTexture = Texture1D::Create();
    m_transferFunctionTexture->SetData(TRANSFER_FUNCTION_TEXTURE_WIDTH_UI, InternalDataFormat::RGBA_32F, PixelDataFormat::RGBA, GraphicsDataType::FLOAT, nullptr);
    m_transferFunctionTexture->SetSampling(Sampling::NEAREST, Sampling::NEAREST);
    m_transferFunctionTexture->SetWrap(WrapMode::CLAMP);
}