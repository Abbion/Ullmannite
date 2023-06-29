#include "Ullpch.h"
#include "FontManager.h"
#include "Logger/Logger.h"
#include "Rendering/Api/Renderer.h"

using namespace Ull;

#define OPENGL_SHADDER_PATH "Resources/Fonts/"

FontManager::~FontManager()
{
	UASSERT(m_fontMap.empty(), "Forgot to unload fonts");
}

void FontManager::LoadFont(const std::string& fontName, const FontTag fontTag)
{
	if (IsFontLoaded(fontTag))
	{
		ULOGE("Font " << fontName << " already loaded");
		return;
	}

	std::string fontPath = OPENGL_SHADDER_PATH + fontName;

	m_fontMap.emplace(std::pair<FontTag, std::unique_ptr<Font>>(fontTag, std::make_unique<Font>(fontPath, m_fontLibrary, 0, 96, 33, 126, 36)));
}

void FontManager::UnloadFont(const FontTag fontTag)
{
	UASSERT(IsFontLoaded(fontTag), "Can't delete font with current tag");
	m_fontMap.erase(fontTag);
}

void FontManager::UnloadAllFonts()
{
	m_fontMap.clear();
}

void FontManager::InitLoader()
{
	UASSERT(FT_Init_FreeType(&m_fontLibrary) == FT_Err_Ok, "Couldin't init font library");
	m_libraryIsAlive = true;

	Renderer::GetInstance().SetPixelUnpackWidth(1);
}

void FontManager::ReleaseLoader()
{
	UASSERT(FT_Done_FreeType(m_fontLibrary) == FT_Err_Ok, "Couldin't release font library");
	m_libraryIsAlive = false;

	Renderer::GetInstance().SetPixelUnpackWidth(4);
}

std::unique_ptr<Font>& FontManager::GetFont(const FontTag fontTag)
{
	UASSERT(IsFontLoaded(fontTag), "Can't get font with current tag");
	return m_fontMap[fontTag];
}

bool FontManager::IsFontLoaded(const FontTag fontTag)
{
	return m_fontMap.find(fontTag) == m_fontMap.end() ? false : true;
}
