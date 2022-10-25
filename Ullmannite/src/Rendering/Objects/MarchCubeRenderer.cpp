#include "Ullpch.h"
#include "MarchCubeRenderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Api/Texture.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/TriangulationTable/TriangulationTable.h"

using namespace Ull;

MarchCubeRenderer::MarchCubeRenderer(const std::string& name, NotOwner<Scene> scene) :
	Node3D(name, scene)
{
	m_cubeMarchShader = ShaderManager::GetInstance().GetShader(ShaderTag::CUBE_MARCH);
	TriangulationTable::GetInstance().CreateTriangulationTable();
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
	m_volumeTexture->SetData(glm::vec3(m_volumeData->width, m_volumeData->height, m_volumeData->depth), InternalDataFormat::R_8I, PixelDataFormat::R_I, GraphicsDataType::USHORT, (void*)volumeData->dataBuffer.data());
	Renderer::GetInstance().SetPixelUnpackWidth(4);
}

void MarchCubeRenderer::GenerateMesh()
{
	auto volumeSize = m_volumeTexture->GetSize();
	std::vector<glm::vec3> meshData(volumeSize.x * volumeSize.y * volumeSize.z * 5);

	m_volumeTexture->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::READ, 0);
	TriangulationTable::GetInstance().GetTriangulationTexture()->BindImage(InternalDataFormat::R_8I, ReadWriteRights::READ, 1);

	m_cubeMarchShader->Bind();
	
	Renderer::GetInstance().DispatchComputeShader(8, 8, 8);

	m_mesh = std::move(meshData);
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