#include "Ullpch.h"
#include "UiText.h"
#include "Resources/ResourceManager.h"
#include "Rendering/Api/Renderer.h"

using namespace Ull;

namespace {
	struct GpuLetterData
	{
		glm::vec3 vertices;
		//glm::vec2 coords;
	};
}

UiText::UiText(const std::string name, const glm::uvec2 position, const glm::uvec2 size, const std::wstring text) :
	UiBasicControl{ name, position, size },
	m_text{ text }
{
}

void UiText::CreateResources()
{
	auto& fontManager = ResourceManager::GetInstance().GetFontMnager();
	const auto characters = fontManager.GetFont(FontTag::UI_FONT)->GenerateDictionary(m_text);
	const auto loadedFontSize = fontManager.GetFont(FontTag::UI_FONT)->GetLoadedHeight();

	glm::vec2 scale{ static_cast<float>(m_fontSize) / static_cast<float>(m_size.x) / static_cast<float>(loadedFontSize),
					 static_cast<float>(m_fontSize) / static_cast<float>(m_size.y) / static_cast<float>(loadedFontSize) };

	std::vector<GpuLetterData> vertices(m_text.length() * 4);
	std::vector<unsigned int> indices(m_text.length() * 6);

	glm::vec3 cursor{ 0.0f, static_cast<float>(loadedFontSize) * scale.y, 0.0f };

	for (unsigned i = 0; i < m_text.length(); ++i)
	{
		const auto letter = m_text[i];
		const auto& character = characters.at(letter);

		cursor.x += character.bearing.x * scale.x;
		vertices[(i * 4)].vertices = cursor;
		cursor.y -= character.bearing.y * scale.y;
		vertices[(i * 4) + 1].vertices = cursor;
		cursor.x += character.size.x * scale.x;
		vertices[(i * 4) + 2].vertices = cursor;
		cursor.y += character.bearing.y * scale.y;
		vertices[(i * 4) + 3].vertices = cursor;
		cursor.x -= (character.size.x + character.bearing.x) * scale.x;
		cursor.x += character.advance * scale.x;

		indices[i * 6] = i * 4;
		indices[(i * 6) + 1] = (i * 4) + 2;
		indices[(i * 6) + 2] = (i * 4) + 1;

		indices[(i * 6) + 3] = (i * 4) + 2;
		indices[(i * 6) + 4] = (i * 4) + 3;
		indices[(i * 6) + 5] = (i * 4);
	}

	m_layout = VertexLayout::Create({
		LayoutElement("Position", GraphicsDataType::FLOAT, 3)
		});

	m_layout->Bind();

	m_vertexBuffer = VertexBuffer::Create(sizeof(GpuLetterData) * vertices.size(), reinterpret_cast<float*>(vertices.data()), GraphicsBufferType::STATIC_DRAW);
	m_indexBuffer = IndexBuffer::Create(sizeof(unsigned int) * indices.size(), indices.data(), GraphicsBufferType::STATIC_DRAW);

	m_layout->Build();
	m_vertexBuffer->Unbind();
	m_layout->Unbind();
}

void UiText::Update()
{
	UiBasicControl::Update();
}

void UiText::Render()
{
	m_shader->Bind();

	m_shader->SetFloat4("color", glm::vec4(1.0f, 0.5f, 1.0f, 1.0f));
	m_shader->SetFloat4x4("modelMatrix", m_perspective * m_modelMatrix);

	m_layout->Bind();

	Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
}