#include "Ullpch.h"
#include "Font.h"
#include "Rendering/Api/Texture.h"

using namespace Ull;

namespace 
{
	constexpr unsigned int TextureDimensions = 512;
}

Font::Font(const std::string& fontPath, FT_Library& library, int width, int height, unsigned firstCharacterIndex, unsigned lastCharacterIndex)
{
	//m_gliphTexture = std::make_shared<Texture2D>(Texture2D::Create());
	m_gliphTexture = Texture2D::Create();
	m_gliphTexture->SetSampling(Sampling::LINEAR, Sampling::LINEAR);
	m_gliphTexture->SetWrap(WrapMode::CLAMP, WrapMode::CLAMP);
	m_gliphTexture->SetStorage(glm::uvec2(TextureDimensions, TextureDimensions), InternalDataFormat::R_8F);

	FT_Face face;

	UASSERT(FT_New_Face(library, fontPath.c_str(), 0, &face) == FT_Err_Ok, "Couldin't load font at " << fontPath);
	UASSERT(FT_Set_Pixel_Sizes(face, width, height) == FT_Err_Ok, "Error accuret when setting font size");

	glm::uvec2 cursor{ 0u, 0u };
	int maxLineHeight = 0;

	for (unsigned i = firstCharacterIndex; i < lastCharacterIndex; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER) != FT_Err_Ok)
		{
			ULOGW("Failed to load gliph at index: " << i);
			continue;
		}

		const glm::uvec2 glyphSize{ face->glyph->bitmap.width, face->glyph->bitmap.rows };

		if (cursor.x + glyphSize.x > TextureDimensions)
		{
			cursor.x = 0;
			cursor.y += maxLineHeight;

			if (cursor.y > TextureDimensions)
			{
				ULOGE("Font texture is too small!");
				return;
			}

			maxLineHeight = 0;
		}

		if (glyphSize.y > maxLineHeight)
		{
			maxLineHeight = glyphSize.y;
		}

		m_gliphTexture->SetSubData(cursor, glyphSize, PixelDataFormat::R, GraphicsDataType::UBYTE, face->glyph->bitmap.buffer);

		Character character{ cursor, glyphSize, glm::uvec2{ face->glyph->bitmap_left, face->glyph->bitmap_top }, face->glyph->advance.x };
		m_characters.insert(std::pair<wchar_t, Character>((wchar_t)i, std::move(character)));

		cursor.x += glyphSize.x;
	}

	FT_Done_Face(face);
}
