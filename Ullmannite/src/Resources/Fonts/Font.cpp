#include "Ullpch.h"
#include "Font.h"
#include "Rendering/Api/Texture.h"
#include "Output/Image2DWriter.h"
#include "Rendering/Api/Renderer.h"
#include <algorithm>

using namespace Ull;

namespace 
{
	constexpr unsigned int TextureDimensions = 2048u;
	constexpr unsigned int GroupSize = TextureDimensions / 16u;
	constexpr unsigned int FontRoundingThreshold = 64;
	constexpr uint16_t max16ui = static_cast<uint16_t>((1L << 16) - 1);

	struct PreGliphLoadInfo
	{
		unsigned id;
		glm::uvec2 size;
	};
}

Font::Font(const std::string& fontPath, FT_Library& library, const int width, const int height, const unsigned firstCharacterIndex, const unsigned lastCharacterIndex, const unsigned sdfOffset)
{
	FT_Face face;

	UASSERT(FT_New_Face(library, fontPath.c_str(), 0, &face) == FT_Err_Ok, "Couldin't load font at " << fontPath);
	UASSERT(FT_Set_Pixel_Sizes(face, width, height) == FT_Err_Ok, "Error accuret when setting font size");

	std::vector<PreGliphLoadInfo> gliphInfoVec(lastCharacterIndex - firstCharacterIndex + 1);

	for (unsigned charId = firstCharacterIndex; charId <= lastCharacterIndex; ++charId)
	{
		if (FT_Load_Char(face, charId, FT_LOAD_RENDER) != FT_Err_Ok)
		{
			ULOGW("Failed to load gliph at index: " << charId);
			continue;
		}

		gliphInfoVec[charId - firstCharacterIndex] = PreGliphLoadInfo{ charId, { face->glyph->bitmap.width, face->glyph->bitmap.rows } };
	}

	std::sort(gliphInfoVec.begin(), gliphInfoVec.end(), [](const PreGliphLoadInfo& gliphInfo_A, const PreGliphLoadInfo& gliphInfo_B) -> bool {
		return gliphInfo_A.size.y > gliphInfo_B.size.y;
	});

	glm::uvec2 cursor{ 0u, 0u };
	unsigned maxLineHeight = 0;

	Texture2D* outerSdf = Texture2D::Create();
	outerSdf->SetSampling(Sampling::NEAREST, Sampling::NEAREST);
	outerSdf->SetWrap(WrapMode::CLAMP, WrapMode::CLAMP);
	outerSdf->SetStorage(glm::uvec2(TextureDimensions, TextureDimensions), InternalDataFormat::R_16UI);

	outerSdf->ClearSubData(glm::uvec2(0.0f, 0.0f), glm::uvec2(TextureDimensions, TextureDimensions), PixelDataFormat::R_I, GraphicsDataType::UINT, &max16ui);

	for (auto& gliphInfo : gliphInfoVec)
	{
		if (FT_Load_Char(face, gliphInfo.id, FT_LOAD_RENDER) != FT_Err_Ok)
		{
			ULOGW("Failed to load gliph at index: " << gliphInfo.id);
			continue;
		}

		const auto& glyphSize = gliphInfo.size;
		const auto sizeWithOffset = glyphSize + (2 * sdfOffset);

		if (cursor.x + sizeWithOffset.x > TextureDimensions)
		{
			cursor.x = 0u;
			cursor.y += maxLineHeight;

			if (cursor.y > TextureDimensions)
			{
				ULOGE("Font texture is too small!");
				return;
			}

			maxLineHeight = 0u;
		}

		if (sizeWithOffset.y > maxLineHeight)
		{
			maxLineHeight = sizeWithOffset.y;
		}

		const unsigned totalSize = glyphSize.x * glyphSize.y;
		std::vector<uint16_t> bitmapBuffer(std::make_move_iterator(face->glyph->bitmap.buffer),
										  std::make_move_iterator(face->glyph->bitmap.buffer + totalSize));

		for (auto& value : bitmapBuffer)
			value = value > 0 ? 0 : max16ui;

		outerSdf->SetSubData(cursor + sdfOffset, glyphSize, PixelDataFormat::R_I, GraphicsDataType::USHORT, bitmapBuffer.data());

		Character character{ cursor + sdfOffset, glyphSize, glm::uvec2{ face->glyph->bitmap_left, face->glyph->bitmap_top }, face->glyph->advance.x };
		m_characters.insert(std::pair<wchar_t, Character>((wchar_t)gliphInfo.id, std::move(character)));

		cursor.x += sizeWithOffset.x;
	}

	FT_Done_Face(face);

	Texture2D* innerSdf = Texture2D::Create();
	innerSdf->SetSampling(Sampling::NEAREST, Sampling::NEAREST);
	innerSdf->SetWrap(WrapMode::CLAMP, WrapMode::CLAMP);
	innerSdf->SetStorage(glm::uvec2(TextureDimensions, TextureDimensions), InternalDataFormat::R_16UI);

	auto& shaderManager = Renderer::GetInstance().GetShaderManager();
	std::shared_ptr<Shader> shader{ nullptr };
	shader = shaderManager.GetShader(ShaderTag::INVERSE_2D_BIT_MAP);

	shader->Bind();
	shader->SetUint("maxValue", max16ui);

	outerSdf->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::READ, 0);
	innerSdf->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::WRITE, 1);

	Renderer::GetInstance().DispatchComputeShader(GroupSize, GroupSize, 1);
	Renderer::GetInstance().GetInstance().Barrier(Renderer::BarrierType::IMAGE_BARRIER);

	const auto maxWidthOfGliphsItr = std::max_element(gliphInfoVec.begin(), gliphInfoVec.end(), [](const PreGliphLoadInfo& gliphInfo_A, const PreGliphLoadInfo& gliphInfo_B) {
		return gliphInfo_A.size.x < gliphInfo_B.size.x;
		});

	const auto maxHeightOfGliphsItr = std::max_element(gliphInfoVec.begin(), gliphInfoVec.end(), [](const PreGliphLoadInfo& gliphInfo_A, const PreGliphLoadInfo& gliphInfo_B) {
		return gliphInfo_A.size.y < gliphInfo_B.size.y;
		});

	const unsigned maxWidthOfGliphs = maxWidthOfGliphsItr != gliphInfoVec.end() ? maxWidthOfGliphsItr->size.x : 0u;
	const unsigned maxHeightOfGliphs = maxHeightOfGliphsItr != gliphInfoVec.end() ? maxHeightOfGliphsItr->size.y : 0u;

	shader = shaderManager.GetShader(ShaderTag::SIGNED_DISTANCE_FIELD_FOR_2D_TEXTURES);
	
	shader->Bind();
	shader->SetInt2("offset", glm::ivec2(0, 1));
	shader->SetUint("textureSize", TextureDimensions);

	outerSdf->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::READ_AND_WRITE, 0);

	for (unsigned i = 0u; i < maxHeightOfGliphs; i++)
	{
		shader->SetUint("beta", (i * 2) + 1);
		Renderer::GetInstance().DispatchComputeShader(GroupSize, GroupSize, 1);
		Renderer::GetInstance().GetInstance().Barrier(Renderer::BarrierType::IMAGE_BARRIER);
	}
	
	shader->SetInt2("offset", glm::ivec2(1, 0));
	for (unsigned i = 0u; i < maxWidthOfGliphs; i++)
	{
		shader->SetUint("beta", (i * 2) + 1);
		Renderer::GetInstance().DispatchComputeShader(GroupSize, GroupSize, 1);
		Renderer::GetInstance().GetInstance().Barrier(Renderer::BarrierType::IMAGE_BARRIER);
	}

	outerSdf->Unbind();

	innerSdf->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::READ_AND_WRITE, 0);

	shader->SetInt2("offset", glm::ivec2(0, 1));
	for (unsigned i = 0u; i < maxHeightOfGliphs / 2u; i++)
	{
		shader->SetUint("beta", (i * 2) + 1);
		Renderer::GetInstance().DispatchComputeShader(GroupSize, GroupSize, 1);
		Renderer::GetInstance().GetInstance().Barrier(Renderer::BarrierType::IMAGE_BARRIER);
	}
	
	shader->SetInt2("offset", glm::ivec2(1, 0));
	for (unsigned i = 0u; i < maxWidthOfGliphs / 2u; i++)
	{
		shader->SetUint("beta", (i * 2) + 1);
		Renderer::GetInstance().DispatchComputeShader(GroupSize, GroupSize, 1);
		Renderer::GetInstance().GetInstance().Barrier(Renderer::BarrierType::IMAGE_BARRIER);
	}
	
	innerSdf->Unbind();

	shader = shaderManager.GetShader(ShaderTag::MEREGE_INNSER_OUTER_SDF);
	shader->Bind();

	outerSdf->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::READ, 0);
	innerSdf->BindImage(InternalDataFormat::R_16UI, ReadWriteRights::READ, 1);
	
	m_gliphTexture = Texture2D::Create();
	m_gliphTexture->SetSampling(Sampling::NEAREST, Sampling::NEAREST);
	m_gliphTexture->SetWrap(WrapMode::CLAMP, WrapMode::CLAMP);
	m_gliphTexture->SetStorage(glm::uvec2(TextureDimensions, TextureDimensions), InternalDataFormat::R_32F);
	m_gliphTexture->BindImage(InternalDataFormat::R_32F, ReadWriteRights::WRITE, 2);

	Renderer::GetInstance().DispatchComputeShader(GroupSize, GroupSize, 1);
	Renderer::GetInstance().GetInstance().Barrier(Renderer::BarrierType::IMAGE_BARRIER);

	FrameBuffer* frameBuffer = FrameBuffer::Create(m_gliphTexture);
	std::vector<float> sdfImage(TextureDimensions * TextureDimensions);
	frameBuffer->GetPixels(glm::uvec2(0, 0), glm::uvec2(TextureDimensions, TextureDimensions), PixelDataFormat::R, GraphicsDataType::FLOAT, sdfImage.data());
	
	auto startName = fontPath.find_last_of('/');
	if (startName == std::string::npos)
		startName = 0;

	const auto endName = fontPath.find_last_of('.');
	const auto fontName = fontPath.substr(startName + 1u, endName - startName - 1u) + "SDF";

	Image2DWriter imageWriter(glm::uvec2(TextureDimensions, TextureDimensions), Image2DWriter::Channels::MONO);
	imageWriter.AddImageData(glm::uvec2(0, 0), glm::uvec2(TextureDimensions, TextureDimensions), sdfImage, 256.0f);
	imageWriter.SaveToFile(fontName);

	//CHECK IF I HAVE TO DELETE frameBuffer and other stuff or the destructor deletes it
}
