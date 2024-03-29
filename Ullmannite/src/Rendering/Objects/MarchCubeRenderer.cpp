#include "Ullpch.h"
#include "MarchCubeRenderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Objects/DirectionalLight.h"
#include "Rendering/TriangulationTable/TriangulationTable.h"
#include "Rendering/Api/OpenGL/BufferOpenGL.h"
#include "Scene/Scene.h"
#include <thread>
#include <chrono>

namespace {
	constexpr unsigned int vertexCounterLocalSize = 8u;

	constexpr uint16_t vertexPositionTextureSize = 2048;

	uint8_t GetAmountOfThreadsForSum(uint64_t toSum)
	{
		if (toSum < 20'000)
			return 1u;
		if (toSum < 50'000)
			return 2u;
		if (toSum < 200'000)
			return 4;
		return 8u;
	}

	unsigned int MINNN = 160;
	unsigned int MAXXX = 5'000;
}

using namespace Ull;

MarchCubeRenderer::MarchCubeRenderer(const std::string& name, NotOwner<Scene> scene) :
	Node3D(name, scene)
{
	m_cubeMarchVertexCounter = ShaderManager::GetInstance().GetShader(ShaderTag::CUBE_MARCH_VERTEX_COUNTER);
	m_cubeMarchShader = ShaderManager::GetInstance().GetShader(ShaderTag::CUBE_MARCH_MESH_GENERATOR);
	m_vertexRendererShader = ShaderManager::GetInstance().GetShader(ShaderTag::CUBE_MARCH_VERTEX_RENDERER);


	TriangulationTable::GetInstance().CreateTriangulationTable();
	TriangulationTable::GetInstance().CreateVectexCountTable();
}

MarchCubeRenderer::~MarchCubeRenderer()
{
	delete m_volumeTexture;
}

void MarchCubeRenderer::SetVolumeData(const std::shared_ptr<VolumeData> volumeData)
{
	m_volumeData = volumeData;
	m_volumeTexture = Texture3D::Create();

	Renderer::GetInstance().SetPixelUnpackWidth(1);
	m_volumeTexture->SetData(glm::vec3(m_volumeData->width, m_volumeData->height, m_volumeData->depth), InternalDataFormat::R_16UI, PixelDataFormat::R_I, GraphicsDataType::USHORT, (void*)volumeData->dataBuffer.data());
	m_volumeTexture->SetSampling(Sampling::NEAREST, Sampling::NEAREST);
	Renderer::GetInstance().SetPixelUnpackWidth(4);
}

void MarchCubeRenderer::SetTransferTexture(NotOwner<Texture1D> transferTexture)
{
	m_transferTexture = transferTexture;
}

void MarchCubeRenderer::GenerateMesh()
{	
	m_vertexCount = CalculateVertexCountGPU();
	const uint16_t amountOfTextures = (uint16_t)std::ceil((double)m_vertexCount / (vertexPositionTextureSize * vertexPositionTextureSize));

	TriangulationTable::GetInstance().GetTriangulationTexture()->BindImage(InternalDataFormat::R_8UI, ReadWriteRights::READ, 1);
	
	if(m_vertexPosTexture != nullptr)
		delete m_vertexPosTexture;

	m_vertexPosTexture = Texture3D::Create();
	m_vertexPosTexture->SetSampling(Sampling::NEAREST, Sampling::NEAREST);
	m_vertexPosTexture->SetWrap(WrapMode::CLAMP, WrapMode::CLAMP, WrapMode::CLAMP);
	m_vertexPosTexture->SetData(glm::uvec3(vertexPositionTextureSize, vertexPositionTextureSize, amountOfTextures), InternalDataFormat::RGBA_32F, PixelDataFormat::RGBA, GraphicsDataType::FLOAT, nullptr);
	m_vertexPosTexture->BindImage(InternalDataFormat::RGBA_32F, ReadWriteRights::WRITE, 2);
	
	uint32_t counter = 0;
	AtomicCounterBuffer* atomicCounter = AtomicCounterBuffer::Create(&counter, sizeof(uint32_t));
	atomicCounter->Bind(3);

	m_cubeMarchShader->Bind();

	//Add one because we need an offset of zeros from every side. In the compute shader we start from -1, -1, -1 so thats the other side
	m_cubeMarchShader->SetUint3("CMsettings.size", glm::uvec3(m_volumeData->width, m_volumeData->height, m_volumeData->depth));
	m_cubeMarchShader->SetUint("CMsettings.minSampleVal", MINNN);
	m_cubeMarchShader->SetUint("CMsettings.maxSampleVal", MAXXX);
	m_cubeMarchShader->SetFloat("CMsettings.maxDataValue", static_cast<float>(m_volumeData->maxValue));

	const unsigned int vertexLocalSizeX = (unsigned int)std::ceil((double)(m_volumeData->width + 2) / (vertexCounterLocalSize * 2));
	const unsigned int vertexLocalSizeY = (unsigned int)std::ceil((double)(m_volumeData->height + 2) / vertexCounterLocalSize);
	const unsigned int vertexLocalSizeZ = (unsigned int)std::ceil((double)(m_volumeData->depth + 2) / vertexCounterLocalSize);

	Renderer::GetInstance().DispatchComputeShader(vertexLocalSizeX, vertexLocalSizeY, vertexLocalSizeZ);
	Renderer::GetInstance().GetInstance().Barrier(Renderer::BarrierType::ATOMIC_COUNTER_BARRIER);
	Renderer::GetInstance().GetInstance().Barrier(Renderer::BarrierType::IMAGE_BARRIER);

	atomicCounter->Unbind();

	m_volumeTexture->Unbind();
	m_vertexPosTexture->Unbind();
	TriangulationTable::GetInstance().GetTriangulationTexture()->Unbind();
	delete atomicCounter;

	if(m_vertexBuffer != nullptr)
		delete m_vertexBuffer;
}

void MarchCubeRenderer::HandleEvent(Event* event)
{

}

void MarchCubeRenderer::Update()
{

}

void MarchCubeRenderer::Render()
{
	auto mainCamera = GetScene()->GetMainCamera();

	m_vertexPosTexture->BindImage(InternalDataFormat::RGBA_32F, ReadWriteRights::READ, 0);
	m_transferTexture->Bind();

	m_vertexRendererShader->Bind();
	m_vertexRendererShader->SetFloat3("materialColor", glm::vec3(0.0, 0.2, 0.5));

	m_vertexRendererShader->SetFloat4x4("projectionMatrix", mainCamera->GetProjectionMatrix());
	m_vertexRendererShader->SetFloat4x4("viewMatrix", mainCamera->GetViewMatrix());

	SetUpLight();

	Renderer::GetInstance().DrawArrays(GraphicsRenderPrimitives::TRIANGLE, (unsigned int)m_vertexCount, 0);

	m_vertexPosTexture->Unbind();
}

uint64_t MarchCubeRenderer::CalculateVertexCountGPU()
{
	auto start = std::chrono::high_resolution_clock::now();

	m_volumeTexture->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::READ, 0);
	TriangulationTable::GetInstance().GetVertexCountTexture()->BindImage(InternalDataFormat::R_8UI, ReadWriteRights::READ, 1);

	//Add two because we need an offset of zeros from every side. In the compute shader we start from -1, -1, -1
	const unsigned int vertexLocalSizeX = (unsigned int)std::ceil((double)(m_volumeData->width + 2) / (vertexCounterLocalSize * 2));
	const unsigned int vertexLocalSizeY = (unsigned int)std::ceil((double)(m_volumeData->height + 2) / vertexCounterLocalSize);
	const unsigned int vertexLocalSizeZ = (unsigned int)std::ceil((double)(m_volumeData->depth + 2) / vertexCounterLocalSize);

	const unsigned int totalSize = vertexLocalSizeX * vertexLocalSizeY * vertexLocalSizeZ;

	uint32_t* vertexCountArr = new uint32_t[totalSize];
	StorageBuffer* storageBuff = StorageBuffer::Create(vertexCountArr, sizeof(uint32_t) * totalSize);

	//Setup shader
	m_cubeMarchVertexCounter->Bind();
	storageBuff->Bind(2);

	m_cubeMarchVertexCounter->SetUint3("CMsettings.size", glm::uvec3(m_volumeData->width, m_volumeData->height, m_volumeData->depth));
	m_cubeMarchVertexCounter->SetUint("CMsettings.minSampleVal", MINNN);
	m_cubeMarchVertexCounter->SetUint("CMsettings.maxSampleVal", MAXXX);
	m_cubeMarchVertexCounter->SetFloat("CMsettings.maxDataValue", static_cast<float>(m_volumeData->maxValue));

	//Run shader
	Renderer::GetInstance().DispatchComputeShader(vertexLocalSizeX, vertexLocalSizeY, vertexLocalSizeZ);
	Renderer::GetInstance().Barrier(Renderer::BarrierType::STORAGE_BUFFER_BARRIER);

	TriangulationTable::GetInstance().GetVertexCountTexture()->Unbind();

	//Get data from shader
	storageBuff->GetData(vertexCountArr, sizeof(uint32_t) * totalSize);	//This takes a lot of time wtf
	storageBuff->Unbind();
	delete storageBuff;

	//Sum all variables from shader
	auto threadNum = GetAmountOfThreadsForSum(totalSize);
	uint64_t totalVertexCount = 0;

	std::vector<std::thread> threads;
	
	if (threadNum > 1)
	{
		std::vector<uint64_t> partialSum(threadNum);
		const auto sumRange = totalSize / threadNum;
		const auto sumRangeLast = totalSize % threadNum;

		for (int i = 0; i < threadNum; ++i)
		{
			threads.push_back(std::thread([&, i]() {
				const auto endItr = sumRange * (i + 1);

				for (unsigned int j = sumRange * i; j < endItr; ++j)
					partialSum[i] += vertexCountArr[j];

				if (i == 0)
				{
					for (unsigned int j = sumRange * threadNum; j < totalSize; ++j)
						partialSum[i] += vertexCountArr[j];
				}
			}));
		}

		for (int i = 0; i < threadNum; ++i)
		{
			threads[i].join();
			totalVertexCount += partialSum[i];
		}
	}
	else
	{
		for (unsigned int i = 0; i < totalSize; ++i)
		{
			totalVertexCount += vertexCountArr[i];
		}
	}

	delete[] vertexCountArr;

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	ULOGD("Triangle count GPU TIME: " << duration.count() << " microseconds");

	return totalVertexCount;
}

uint64_t MarchCubeRenderer::CalculateVertexCountCPU()
{
	auto start = std::chrono::high_resolution_clock::now();

	const auto minSampleVal = 10;
	const auto maxSampleVal = 16'000;

	size_t totalVertexCount = 0u;

	const auto yOffset = m_volumeData->width;
	const auto zOffset = m_volumeData->width * m_volumeData->height;

	auto vertexCountTable = TriangulationTable::GetInstance().GetVertexCountTable();

	for(size_t z = 0; z < m_volumeData->depth - 1; ++z)
	{
		for(size_t y = 0; y < m_volumeData->height - 1; ++y)
		{
			for(size_t x = 0; x < m_volumeData->width - 1; ++x)
			{
				auto posInArray = x + (y * m_volumeData->width) + (z * m_volumeData->width * m_volumeData->height);

				std::vector<size_t> samplePositions(8);
				samplePositions[0] = posInArray;
				samplePositions[1] = posInArray + 1;
				samplePositions[2] = posInArray + yOffset + 1;
				samplePositions[3] = posInArray + yOffset;

				samplePositions[4] = posInArray + zOffset;
				samplePositions[5] = posInArray + zOffset + 1;
				samplePositions[6] = posInArray + zOffset + yOffset + 1;
				samplePositions[7] = posInArray + zOffset + yOffset;
					
				uint16_t activeEdgeCounter = 0u;

				for(unsigned int i = 0; i < 8; ++i)
				{
					auto samplePos = samplePositions[i];
					auto cornerValue = m_volumeData->dataBuffer[samplePos];
					
					if(cornerValue >= minSampleVal && cornerValue <= maxSampleVal)
					{
						activeEdgeCounter |= 1 << i;
					}
				}
				
				totalVertexCount += vertexCountTable[activeEdgeCounter];
			}
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	ULOGD("Triangle count CPU TIME: " << duration.count() << " microseconds");

	return totalVertexCount;
}

void MarchCubeRenderer::SetUpLight()
{
	const auto dirLightNode = m_scene->GetNodeByName("dirLight");
	
	if(dirLightNode == nullptr)
	{
		m_vertexRendererShader->SetFloat3("lightSettings.lightDir", glm::vec3(1.0f, -1.0f, 0.0f));
		m_vertexRendererShader->SetFloat3("lightSettings.lightColor", glm::vec3(0.2f, 0.2f, 0.2f));
		m_vertexRendererShader->SetFloat("lightSettings.ambientStrength", 0.5f);
	}
	else
	{
		const auto dirLight = static_cast<DirectionalLight*>(dirLightNode);

		m_vertexRendererShader->SetFloat3("lightSettings.lightDir", dirLight->GetDirection());
		m_vertexRendererShader->SetFloat3("lightSettings.lightColor", dirLight->GetLightColor());
		m_vertexRendererShader->SetFloat("lightSettings.ambientStrength", dirLight->GetAmbientStrength());
	}
}