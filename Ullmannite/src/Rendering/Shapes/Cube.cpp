#include "Ullpch.h"
#include "Cube.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/ShaderManager.h"

using namespace Ull;

Cube::Cube(const std::string& name, const Scene* scene) :
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
		//Front
		-0.5f, 0.5f, -2.0f,
		0.5f, 0.5f, -2.0f,
		-0.5f, -0.5f, -2.0f,
		0.5f, -0.5f, -2.0f
    };

	unsigned int indices[] = {
		0, 1, 2,
		1, 3, 2
	};

	m_layout = VertexLayout::Create({LayoutElement("Position", GraphicsDataType::FLOAT, 3)});

	m_layout->Bind();

	m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
	m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

	m_layout->Build();
	m_vertexBuffer->Unbind();
	m_layout->Unbind();
}

void Cube::Render()
{

	m_shader->Bind();

	auto mainCamera = GetScene()->GetMainCamera();

	m_shader->SetFloat4("shaderColor", m_color);
	m_shader->SetFloat4x4("projectionMatrix", mainCamera->GetProjectionMatrix());
	m_shader->SetFloat4x4("viewMatrix", mainCamera->GetViewMatrix());

	m_layout->Bind();

	Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
}
