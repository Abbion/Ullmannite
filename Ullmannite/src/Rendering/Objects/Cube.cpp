#include "Ullpch.h"
#include "Cube.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Objects/DirectionalLight.h"

using namespace Ull;

Cube::Cube(const std::string& name, NotOwner<Scene> scene) :
	Node3D(name, scene)
{
	m_shader = ShaderManager::GetInstance().GetShader(ShaderTag::MARKER);
	CreateResources();
}

void Cube::CreateResources()
{
	if (m_vertexBuffer != nullptr)
		delete m_vertexBuffer;

	if (m_indexBuffer != nullptr)
		delete m_indexBuffer;

	if (m_layout != nullptr)
		delete m_layout;

    float vertices[] = {
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f
    };

	unsigned int indices[] = {
		0, 2, 1,
		1, 2, 3,
		
		0, 4, 2,
		2, 4, 6,

		1, 3, 5,
		3, 7, 5,

		4, 5, 6,
		5, 7, 6,

		0, 1, 4,
		1, 5, 4,

		2, 6, 3,
		3, 6, 7 
	};

	m_layout = VertexLayout::Create({LayoutElement("Position", GraphicsDataType::FLOAT, 3), LayoutElement("Normal", GraphicsDataType::FLOAT, 3)});

	m_layout->Bind();

	m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
	m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

	m_layout->Build();
	m_vertexBuffer->Unbind();
	m_layout->Unbind();

	m_scene->SetUpdated(true);
}

void Cube::Update()
{

}

void Cube::Render()
{
	m_shader->Bind();

	auto mainCamera = GetScene()->GetMainCamera();

	m_transformationMatrix = glm::identity<glm::mat4>();
	m_transformationMatrix = glm::translate(m_transformationMatrix, m_position);


	m_shader->SetFloat3("materialColor", m_color);

	m_shader->SetFloat4x4("modelMatrix", m_transformationMatrix);
	m_shader->SetFloat4x4("projectionMatrix", mainCamera->GetProjectionMatrix());
	m_shader->SetFloat4x4("viewMatrix", mainCamera->GetViewMatrix());

	m_layout->Bind();

	Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
}
