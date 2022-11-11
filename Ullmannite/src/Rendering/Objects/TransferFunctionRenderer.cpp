#include "Ullpch.h"
#include "TransferFunctionRenderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/Buffer.h"
#include <algorithm>

using namespace Ull;

TransferFunctionRenderer::TransferFunctionRenderer()
{
    Init();
}

TransferFunctionRenderer::TransferFunctionRenderer(const std::vector<TransferPoint>& transferPoints) :
    m_transferPoints(transferPoints)
{
    Init();
}

TransferFunctionRenderer::~TransferFunctionRenderer()
{
    if(m_transferFunctionTexture != nullptr)
        delete m_transferFunctionTexture;
}

void TransferFunctionRenderer::AddPoint(const TransferPoint& point)
{
    m_transferPoints.push_back(point);
}

void TransferFunctionRenderer::EditPoint(const TransferPoint& point)
{
    auto size = m_transferPoints.size();
    
    for (unsigned int i = 0; i < size; ++i)
    {
        if(m_transferPoints[i].position == point.position)
        {
            m_transferPoints[i] = point;
            return;
        }
    }
}

void TransferFunctionRenderer::DeletePoint(uint16_t position)
{
    auto size = m_transferPoints.size();

    for(unsigned int i = 0; i < size; ++i)
    {
        if(m_transferPoints[i].position == position)
        {
            m_transferPoints.erase(m_transferPoints.begin() + i);
            return;
        }
    }
}

void TransferFunctionRenderer::GenerateTransferFunction()
{
    std::sort(m_transferPoints.begin(), m_transferPoints.end(), [](const TransferPoint& a, const TransferPoint& b){ return a.position < b.position; });

    auto size = m_transferPoints.size();

    StorageBuffer* transferPointsBuffer = StorageBuffer::Create(m_transferPoints.data(), sizeof(TransferPoint) * size);

    m_transferFunctionTexture->BindImage(InternalDataFormat::RGBA_32F, ReadWriteRights::READ, 0);
    transferPointsBuffer->Bind(1);

    m_transferFunctionGeneratorShader->Bind();
    m_transferFunctionGeneratorShader->SetUint("colorPointsCount", size);

    Renderer::GetInstance().DispatchComputeShader(1, 1, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    
    m_transferFunctionTexture->Unbind();

    delete transferPointsBuffer;
}

void TransferFunctionRenderer::Init()
{
    m_transferFunctionGeneratorShader = ShaderManager::GetInstance().GetShader(ShaderTag::TRANSFER_FUNCTION_GENERATOR);
    
    m_transferFunctionTexture = Texture1D::Create();
    m_transferFunctionTexture->SetData(512, InternalDataFormat::RGBA_32F, PixelDataFormat::RGBA, GraphicsDataType::FLOAT, nullptr);
    m_transferFunctionTexture->SetSampling(Sampling::NEAREST, Sampling::NEAREST);
    m_transferFunctionTexture->SetWrap(WrapMode::CLAMP);
}