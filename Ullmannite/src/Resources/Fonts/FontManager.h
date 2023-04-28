#pragma once
#include "Ullpch.h"
#include "Font.h"
#include <map>

namespace Ull
{
	enum class FontTag
	{
		UI_FONT,
	};

	class ResourceManager;

	class FontManager 
	{
	public:
		NON_COPYABLE(FontManager);

		~FontManager();

		std::unique_ptr<Font>& GetFont(const FontTag fontTag);

		void LoadFont(const std::string& fontName, const FontTag fontTag);
		void UnloadFont(const FontTag fontTag);
		void UnloadAllFonts();

		void InitLoader();
		void ReleaseLoader();

	private:
		FontManager() = default;

		bool IsFontLoaded(const FontTag fontTag);

		FT_Library m_fontLibrary;
		bool m_libraryIsAlive{ false };

		std::map<FontTag, std::unique_ptr<Font>> m_fontMap;

		friend class ResourceManager;
	};
}