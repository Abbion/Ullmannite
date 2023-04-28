#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <Rendering/Api/Texture.h>

namespace Ull
{
	class Font 
	{
	public:
		struct Character
		{
			glm::uvec2 position;
			glm::uvec2 size;
			glm::uvec2 bearing;
			unsigned int advance;
		};

	public:
		Font(const std::string& fontPath, FT_Library& library, int width, int height, unsigned firstCharacterIndex, unsigned lastCharacterIndex);

	private:
		//std::shared_ptr<Texture2D> m_gliphTexture;
		Texture2D* m_gliphTexture;
		std::map<wchar_t, Character> m_characters;
	};
}