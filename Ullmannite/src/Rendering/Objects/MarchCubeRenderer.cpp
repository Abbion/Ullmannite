#include "Ullpch.h"
#include "MarchCubeRenderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/TriangulationTable/TriangulationTable.h"
#include "Rendering/Api/OpenGL/BufferOpenGL.h"
#include <thread>
#include <chrono>

namespace {
	constexpr unsigned int vertexCounterLocalSize = 8u;

	constexpr uint8_t vertexCountTable[256] = 
	{ 0, 3, 3, 6, 3, 6, 6, 9, 3, 6, 6, 9, 6, 9, 9, 6, 3,
	  6, 6, 9, 6, 9, 9, 12, 6, 9, 9, 12, 9, 12, 12, 9, 3,
	  6, 6, 9, 6, 9, 9, 12, 6, 9, 9, 12, 9, 12, 12, 9, 6,
	  9, 9, 6, 9, 12, 12, 9, 9, 12, 12, 9, 12, 15, 15, 6,
	  3, 6, 6, 9, 6, 9, 9, 12, 6, 9, 9, 12, 9, 12, 12, 9,
	  6, 9, 9, 12, 9, 12, 12, 15, 9, 12, 12, 15, 12, 15,
	  15, 12, 6, 9, 9, 12, 9, 12, 6, 9, 9, 12, 12, 15, 12,
	  15, 9, 6, 9, 12, 12, 9, 12, 15, 9, 6, 12, 15, 15, 12,
	  15, 6, 12, 3, 3, 6, 6, 9, 6, 9, 9, 12, 6, 9, 9, 12, 9,
	  12, 12, 9, 6, 9, 9, 12, 9, 12, 12, 15, 9, 6, 12, 9, 12,
	  9, 15, 6, 6, 9, 9, 12, 9, 12, 12, 15, 9, 12, 12, 15, 12,
	  15, 15, 12, 9, 12, 12, 9, 12, 15, 15, 12, 12, 9, 15, 6, 15,
	  12, 6, 3, 6, 9, 9, 12, 9, 12, 12, 15, 9, 12, 12, 15, 6, 9, 9,
	  6, 9, 12, 12, 15, 12, 15, 15, 6, 12, 9, 15, 12, 9, 6, 12, 3,
	  9, 12, 12, 15, 12, 15, 9, 12, 12, 15, 15, 6, 9, 12, 6, 3, 6,
	  9, 9, 6, 9, 12, 6, 3, 9, 6, 12, 3, 6, 3, 3, 0 };

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
}

using namespace Ull;

MarchCubeRenderer::MarchCubeRenderer(const std::string& name, NotOwner<Scene> scene) :
	Node3D(name, scene)
{
	m_cubeMarchVertexCounter = ShaderManager::GetInstance().GetShader(ShaderTag::CUBE_MARCH_VERTEX_COUNTER);
	m_cubeMarchShader = ShaderManager::GetInstance().GetShader(ShaderTag::CUBE_MARCH);


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

void MarchCubeRenderer::GenerateMesh()
{	
	auto vertexCount = GetVertexCountGPU();
	auto vertexCount2 = GetVertexCountCPU();

	int64_t diff = vertexCount - vertexCount2;

	auto volumeSize = m_volumeTexture->GetSize();
	
	//std::vector<glm::vec3> meshData(volumeSize.x * volumeSize.y * volumeSize.z * 5);

	TriangulationTable::GetInstance().GetTriangulationTexture()->BindImage(InternalDataFormat::R_8I, ReadWriteRights::READ, 1);

	m_cubeMarchShader->Bind();
	
	Renderer::GetInstance().DispatchComputeShader(8, 8, 8);


	//m_mesh = std::move(meshData);
}

void MarchCubeRenderer::HandleEvent(Event* event)
{

}

void MarchCubeRenderer::Update()
{

}

void MarchCubeRenderer::Render()
{

}

uint64_t MarchCubeRenderer::GetVertexCountGPU()
{
	auto start = std::chrono::high_resolution_clock::now();

	uint8_t sampleJump = 1;

	m_volumeTexture->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::READ, 0);
	TriangulationTable::GetInstance().GetVertexCountTexture()->BindImage(InternalDataFormat::R_8UI, ReadWriteRights::READ, 1);

	const unsigned int vertexLocalSizeX = (unsigned int)std::ceil((double)m_volumeData->width / (vertexCounterLocalSize * 2 * sampleJump));
	const unsigned int vertexLocalSizeY = (unsigned int)std::ceil((double)m_volumeData->height / (vertexCounterLocalSize * sampleJump));
	const unsigned int vertexLocalSizeZ = (unsigned int)std::ceil((double)m_volumeData->depth / (vertexCounterLocalSize * sampleJump));
	const unsigned int totalSize = vertexLocalSizeX * vertexLocalSizeY * vertexLocalSizeZ;

	uint32_t* vertexCountArr = new uint32_t[totalSize];
	memset(vertexCountArr, 0, sizeof(uint32_t) * totalSize);
	StorageBuffer* storageBuff = StorageBuffer::Create(vertexCountArr, sizeof(uint32_t) * totalSize);
	storageBuff->Bind(2);
	
	//Setup shader
	m_cubeMarchVertexCounter->Bind();

	m_cubeMarchVertexCounter->SetUint3("CMsettings.size", glm::uvec3(m_volumeData->width, m_volumeData->height, m_volumeData->depth));
	m_cubeMarchVertexCounter->SetUint("CMsettings.sampleJump", sampleJump);
	m_cubeMarchVertexCounter->SetUint("CMsettings.minSampleVal", 10);
	m_cubeMarchVertexCounter->SetUint("CMsettings.maxSampleVal", 16'000);

	//Run shader
	Renderer::GetInstance().DispatchComputeShader(vertexLocalSizeX, vertexLocalSizeY, vertexLocalSizeZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//Get data from shader
	storageBuff->GetData(vertexCountArr, sizeof(uint32_t) * totalSize);
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

		for (int i = 0; i < threadNum; i++)
		{
			threads[i].join();
			totalVertexCount += partialSum[i];
		}
	}
	else
	{
		for (unsigned int i = 0; i < totalSize; ++i)
			totalVertexCount += vertexCountArr[i];
	}

	delete[] vertexCountArr;

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	ULOGD("Triangle count GPU TIME: " << duration.count() << " microseconds");

	return totalVertexCount;
}

uint64_t MarchCubeRenderer::GetVertexCountCPU()
{
	auto start = std::chrono::high_resolution_clock::now();

	const auto minSampleVal = 10;
	const auto maxSampleVal = 16'000;

	size_t totalVertexCount = 0u;

	const auto yOffset = m_volumeData->width;
	const auto zOffset = m_volumeData->width * m_volumeData->height;

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
				samplePositions[2] = posInArray + yOffset;
				samplePositions[3] = posInArray + yOffset + 1;

				samplePositions[4] = posInArray + zOffset;
				samplePositions[5] = posInArray + zOffset + 1;
				samplePositions[6] = posInArray + zOffset + yOffset;
				samplePositions[7] = posInArray + zOffset + yOffset + 1;
					
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
