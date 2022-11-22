#include "Ullpch.h"
#include "MarchCubeRenderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Objects/DirectionalLight.h"
#include "Rendering/TriangulationTable/TriangulationTable.h"
#include "Rendering/Api/OpenGL/BufferOpenGL.h"
#include "Event/EventAggregator.h"
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

	constexpr float InitialMinThresholdRate = 0.05f;
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

	glm::uvec2 initThresholds((unsigned int)(InitialMinThresholdRate * (float)m_volumeData->maxValue), m_volumeData->maxValue);
	
	if(initThresholds.x < 1)
		initThresholds.x = 1;

	m_thresholds = initThresholds;
	m_cuttingSettingsInt = glm::ivec3(m_volumeData->width, m_volumeData->height, m_volumeData->depth);

	EventAggregator::Publish(std::make_shared<ExaminationThresholdChangedEvent>(EventType::ExaminationThresholdChanged, initThresholds));
	m_thresholdInitEventShip = true;
}

void MarchCubeRenderer::SetTransferTexture(NotOwner<Texture1D> transferTexture)
{
	m_transferTexture = transferTexture;
}

void MarchCubeRenderer::GenerateMesh()
{	
	if (m_volumeData == nullptr)
		return;

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
	m_cubeMarchShader->SetUint("CMsettings.minSampleVal", (unsigned)m_thresholds.x);
	m_cubeMarchShader->SetUint("CMsettings.maxSampleVal", (unsigned)m_thresholds.y);
	m_cubeMarchShader->SetFloat("CMsettings.maxDataValue", static_cast<float>(m_volumeData->maxValue));
	m_cubeMarchShader->SetInt3("cuttingPlanes", m_cuttingSettingsInt);

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
	switch (event->GetType())
	{
	case EventType::ExaminationThresholdChanged:
		if(m_thresholdInitEventShip)
		{
			m_thresholdInitEventShip = false;
			break;
		}

		auto newThresholds = static_cast<ExaminationThresholdChangedEvent*>(event)->GetVal();
		m_thresholds.x = (unsigned int)newThresholds.x;
		m_thresholds.y = (unsigned int)newThresholds.y;
		GenerateMesh();
		m_scene->SetUpdated(true);
		break;
	
	case EventType::CuttingSettingsChanged:
		auto cuttingSettings = static_cast<CuttingSettingsChangedEvent*>(event)->GetVal();

		if (m_volumeData != nullptr)
		{
			m_cuttingSettingsInt = { (cuttingSettings.cuttingPositions.x / 100.0f) * m_volumeData->width, (cuttingSettings.cuttingPositions.y / 100.0f) * m_volumeData->height, (cuttingSettings.cuttingPositions.z / 100.0f) * m_volumeData->depth };
			m_cuttingSettingsInt.x = cuttingSettings.invertedAxis[0] ? -m_cuttingSettingsInt.x : m_cuttingSettingsInt.x;
			m_cuttingSettingsInt.y = cuttingSettings.invertedAxis[1] ? -m_cuttingSettingsInt.y : m_cuttingSettingsInt.y;
			m_cuttingSettingsInt.z = cuttingSettings.invertedAxis[2] ? -m_cuttingSettingsInt.z : m_cuttingSettingsInt.z;
		}

		GenerateMesh();
		event->MarkHandeled(true);
		m_scene->SetUpdated(true);
	break;
	}
}

void MarchCubeRenderer::Update()
{

}

void MarchCubeRenderer::Render()
{
	if (m_volumeData == nullptr)
		return;

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
	m_cubeMarchVertexCounter->SetUint("CMsettings.minSampleVal", (unsigned)m_thresholds.x);
	m_cubeMarchVertexCounter->SetUint("CMsettings.maxSampleVal", (unsigned)m_thresholds.y);
	m_cubeMarchVertexCounter->SetFloat("CMsettings.maxDataValue", static_cast<float>(m_volumeData->maxValue));
	m_cubeMarchVertexCounter->SetInt3("cuttingPlanes", m_cuttingSettingsInt);

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
	//ULOGD("Triangle count GPU TIME: " << duration.count() << " microseconds");

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