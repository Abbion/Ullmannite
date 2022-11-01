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
	m_cubeMarchVertexCounter->SetUint("CMsettings.maxSampleVal", 160'000);

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
