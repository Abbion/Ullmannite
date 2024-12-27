#include "Ullpch.h"
#include "UiText.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Resources/ResourceManager.h"
#include "Rendering/Api/Renderer.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace Ull;

namespace {
	constexpr auto AdvanceCharacter = L'i';
	constexpr auto F_FontTextureDimensions = static_cast<float>(Ull::FontTextureDimensions);

	struct GpuLetterData
	{
		glm::vec3 position;
		glm::vec2 coords;
	};
}

UiText::UiText(const std::string name, const glm::vec2 position, const glm::vec2 size, const std::wstring text) :
	UiBasicControl{ name, position, size, UiControlType::UiText },
	m_text{ text }
{
	m_textAlignmentlMatrix = glm::mat4x4(1.0f);

	auto& shaderManager = Renderer::GetInstance().GetShaderManager();
	m_shader = shaderManager.GetShader(ShaderTag::SDF_TEXT);
}

glm::uvec2 UiText::GetTextSize()
{
	if (m_updateDrawData)
		Update();

	return glm::uvec2(static_cast<unsigned>(m_cursorPos.x), static_cast<unsigned>(m_cursorPos.y));
}

void UiText::SetSize(const glm::uvec2 size)
{
	UiElement::SetSize(size);
	m_updateDrawData = true;
}

void UiText::SetString(std::wstring& text)
{
	m_text = text;
	m_updateDrawData = true;
}

void UiText::SetFont(const FontTag font)
{
	m_fontTag = font;
	m_updateDrawData = true;
}

void UiText::SetColor(const glm::vec4 color)
{
	m_color = color;
}

void UiText::SetFontSize(const unsigned size)
{
	m_fontSize = size;
	m_updateDrawData = true;
}

void UiText::SetSpaceing(const float spaceing)
{
	m_spaceing = spaceing;
	m_updateDrawData = true;
}

void UiText::SetLeading(const float leading)
{
	m_leading = leading;
	m_updateDrawData = true;
}

void UiText::SetAlignment(const HorizontalAlignment hAlignment, const VerticalAlignment vAlignment)
{
	m_horizontalAlignment = hAlignment;
	m_verticalAlignment = vAlignment;
	m_updateDrawData = true;
}

void UiText::SetEdgeSmoothing(const float smoothing)
{
	m_smoothing = smoothing;
	m_updateDrawData = true;
}

void UiText::SetSampleThreshold(const float threshold)
{
	m_threshold = threshold;
}

void UiText::SetSmoothingExceptance(std::initializer_list<wchar_t> exceptance)
{
	m_smoothingExceptance.clear();
	m_smoothingExceptance = exceptance;
}

void UiText::CreateResources()
{
	if (m_vertexBuffer != nullptr)
		delete m_vertexBuffer;

	if (m_indexBuffer != nullptr)
		delete m_indexBuffer;

	if (m_layout != nullptr)
		delete m_layout;

	auto& fontManager = ResourceManager::GetInstance().GetFontMnager();
	const auto characters = fontManager.GetFont(m_fontTag)->GenerateDictionary(m_text);
	const auto loadedFontSize = fontManager.GetFont(m_fontTag)->GetLoadedHeight();
	const auto spaceWidth = fontManager.GetFont(m_fontTag)->GetCharacter(AdvanceCharacter).advance;

	const auto size = GetSize();

	if (size.x == 0 || size.y == 0)
		return;

	const glm::vec2 scale{ static_cast<float>(m_fontSize) / static_cast<float>(size.x) / static_cast<float>(loadedFontSize),
					 static_cast<float>(m_fontSize) / static_cast<float>(size.y) / static_cast<float>(loadedFontSize) };

	//TODO: Subtract whitespaces form the text length
	std::vector<GpuLetterData> vertices((m_text.length()) * 4);
	std::vector<unsigned int> indices(m_text.length() * 6);

	m_cursorPos = glm::vec3(0.0f, static_cast<float>(loadedFontSize) * scale.y, 0.0f);
	m_displayTextSize = glm::vec2{ 0.0f, 0.0f };

	auto displayTextSizeMin = glm::vec2{ 0.0f, 0.0f };
	auto displayTextSizeMax = glm::vec2{ 0.0f, 0.0f };

	for (unsigned i = 0; i < m_text.length(); ++i)
	{
		const auto letter = m_text[i];

		if (letter == L' ')
		{
			m_cursorPos.x += spaceWidth * scale.x;
			continue;
		}
		else if (letter == L'\n')
		{
			m_cursorPos.y += (loadedFontSize * m_leading) * scale.y;
			m_cursorPos.x = 0.0f;
			continue;
		}

		float enableSmoothing = 1.0f;		
		for (const auto exceptance : m_smoothingExceptance)
		{
			if (letter == exceptance)
			{
				enableSmoothing = 0.0;
				break;
			}
		}

		const auto& character = characters.at(letter);

		//Left down
		m_cursorPos.x += character.bearing.x * scale.x;
		m_cursorPos.y += (character.size.y - character.bearing.y) * scale.y;
		vertices[(i * 4)].position = m_cursorPos;
		vertices[(i * 4)].coords = glm::vec2{ (character.position.x - m_smoothing * enableSmoothing) / F_FontTextureDimensions,
											  (character.position.y + character.size.y + m_smoothing * enableSmoothing) / F_FontTextureDimensions };

		//Left Up
		m_cursorPos.y -= character.size.y * scale.y;
		vertices[(i * 4) + 1].position = m_cursorPos;
		vertices[(i * 4) + 1].coords = glm::vec2{ (character.position.x - m_smoothing * enableSmoothing) / F_FontTextureDimensions,
												  (character.position.y - m_smoothing * enableSmoothing) / F_FontTextureDimensions };

		//Right Up
		m_cursorPos.x += character.size.x * scale.x;
		vertices[(i * 4) + 2].position = m_cursorPos;
		vertices[(i * 4) + 2].coords = glm::vec2{ (character.position.x + character.size.x + m_smoothing * enableSmoothing) / F_FontTextureDimensions,
												  (character.position.y - m_smoothing * enableSmoothing) / F_FontTextureDimensions };

		//Right Down
		m_cursorPos.y += character.size.y * scale.y;
		vertices[(i * 4) + 3].position = m_cursorPos;
		vertices[(i * 4) + 3].coords = glm::vec2{ (character.position.x + character.size.x + m_smoothing * enableSmoothing) / F_FontTextureDimensions,
												  (character.position.y + character.size.y + m_smoothing * enableSmoothing) / F_FontTextureDimensions };

		//Left Down of next character
		m_cursorPos.y -= (character.size.y - character.bearing.y) * scale.y;
		m_cursorPos.x -= (character.size.x + character.bearing.x) * scale.x;
		m_cursorPos.x += (character.advance + m_spaceing) * scale.x;

		indices[i * 6] = i * 4;
		indices[(i * 6) + 1] = (i * 4) + 2;
		indices[(i * 6) + 2] = (i * 4) + 1;

		indices[(i * 6) + 3] = (i * 4) + 2;
		indices[(i * 6) + 4] = (i * 4) + 3;
		indices[(i * 6) + 5] = (i * 4);
	}

	auto topLeftCornerOfTextDisplayArea = vertices[0].position;
	auto bottomRightCornderOfTextDisplayArea = vertices[0].position;

	for (auto& vertex : vertices)
	{
		topLeftCornerOfTextDisplayArea.x = min(topLeftCornerOfTextDisplayArea.x, vertex.position.x);
		topLeftCornerOfTextDisplayArea.y = min(topLeftCornerOfTextDisplayArea.y, vertex.position.y);

		bottomRightCornderOfTextDisplayArea.x = max(bottomRightCornderOfTextDisplayArea.x, vertex.position.x);
		bottomRightCornderOfTextDisplayArea.y = max(bottomRightCornderOfTextDisplayArea.y, vertex.position.y);
	}

	const auto displayArea = bottomRightCornderOfTextDisplayArea - topLeftCornerOfTextDisplayArea;
	m_displayTextSize = glm::vec2{ displayArea.x / scale.x, displayArea.y / scale.y };
	m_displayTextSize = glm::vec2{ (m_displayTextSize.x / loadedFontSize) * m_fontSize, (m_displayTextSize.y / loadedFontSize) * m_fontSize };

	m_displayTextCornderOffset = glm::vec2{ topLeftCornerOfTextDisplayArea.x / scale.x, topLeftCornerOfTextDisplayArea.y / scale.y };
	m_displayTextCornderOffset = glm::vec2{ (m_displayTextCornderOffset.x / loadedFontSize) * m_fontSize, (m_displayTextCornderOffset.y / loadedFontSize) * m_fontSize };

	m_layout = VertexLayout::Create({
		LayoutElement("Position", GraphicsDataType::FLOAT, 3),
		LayoutElement("TextureCoordinates", GraphicsDataType::FLOAT, 2)
	});

	m_layout->Bind();

	m_vertexBuffer = VertexBuffer::Create(sizeof(GpuLetterData) * vertices.size(), reinterpret_cast<float*>(vertices.data()), GraphicsBufferType::STATIC_DRAW);
	m_indexBuffer = IndexBuffer::Create(static_cast<int>(sizeof(unsigned int) * indices.size()), indices.data(), GraphicsBufferType::STATIC_DRAW);

	m_layout->Build();
	m_vertexBuffer->Unbind();
	m_layout->Unbind();
}

void UiText::Update()
{
	if (m_updateDrawData)
	{
		CreateResources();
		RealignText();
		m_updateDrawData = false;
	}

	UiBasicControl::Update();
}

void UiText::Render()
{
	if (!IsVisible())
		return;

	m_shader->Bind();
	m_shader->SetFloat4x4("modelMatrix", m_perspective * m_textAlignmentlMatrix * GetTransform());
	m_shader->SetFloat("smoothing", m_smoothing);
	m_shader->SetFloat("threshold", m_threshold);
	m_shader->SetFloat4("color", m_color);

	auto parent = GetParent();
	auto position = GetPosition();
	const auto size = GetSize();

	while (parent != nullptr)
	{
		if(parent->GetType() != UiElementType::RenderArea)
			position += parent->GetPosition();
		else
		{
			position.y = parent->GetSize().y - position.y;
			break;
		}
			
		parent = parent->GetParent();
	}

	m_shader->SetFloat2("renderAreaPosition", position);
	m_shader->SetFloat2("renderAreaSize", GetSize());

	auto& fontManager = ResourceManager::GetInstance().GetFontMnager();
	const auto texture = fontManager.GetFont(m_fontTag)->GetTexture();
	texture->Bind();

	m_layout->Bind();

	Renderer::GetInstance().SetBlending(true);
	Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
	Renderer::GetInstance().SetBlending(false);
}

void UiText::UpdateDisplayTextSize()
{

}

void UiText::RealignText()
{
	float horizontalOffset = 0.0f;
	float verticalOffset = 0.0f;
	const auto size = GetSize();

	switch (m_horizontalAlignment)
	{
	case HorizontalAlignment::LEFT:
		horizontalOffset = -m_displayTextCornderOffset.x;
		break;
	case HorizontalAlignment::CENTER:
		horizontalOffset = (size.x / 2.0f) - (m_displayTextSize.x / 2.0f) - m_displayTextCornderOffset.x;
		break;
	case HorizontalAlignment::RIGHT:
		horizontalOffset = size.x - m_displayTextSize.x -m_displayTextCornderOffset.x;
		break;
	}

	switch (m_verticalAlignment)
	{
	case VerticalAlignment::TOP:
		verticalOffset = -m_displayTextCornderOffset.y;
		break;
	case VerticalAlignment::CENTER:
		verticalOffset = (size.y / 2.0f) - (m_displayTextSize.y / 2.0f) - m_displayTextCornderOffset.y;
		break;
	case VerticalAlignment::BOTTOM:
		verticalOffset = size.y - m_displayTextSize.y - m_displayTextCornderOffset.y;
		break;
	}

	m_textAlignmentlMatrix = glm::mat4x4(1.0f);
	m_textAlignmentlMatrix = glm::translate(m_textAlignmentlMatrix, glm::vec3(horizontalOffset, verticalOffset, 0.0f));
}