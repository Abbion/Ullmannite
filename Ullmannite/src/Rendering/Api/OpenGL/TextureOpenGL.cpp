#include "Ullpch.h"
#include "TextureOpenGL.h"
#include "glad/glad.h"
#include "Logger/Logger.h"

using namespace Ull;

namespace
{
	constexpr GLenum ConverterChannel(ColorFormat channel)
	{
		switch (channel)
		{
		case ColorFormat::R:
			return GL_RED;
		break;

		case ColorFormat::RG:
			return GL_RG;
		break;

		case ColorFormat::RGB:
			return GL_RGB;
		break;

		case ColorFormat::RGBA:
			return GL_RGBA;
		break;

		default:
			ULOGE("Can't conver this channel");
            return 0;
        break;
		}
	}

	constexpr GLenum ConverterWrap(WrapMode wrap)
	{
		switch(wrap)
		{
		case WrapMode::BORDER:
			return GL_CLAMP_TO_BORDER;
		break;

		case WrapMode::CLAMP:
			return GL_CLAMP_TO_EDGE;
		break;

		case WrapMode::MIRROR_REPEAT:
			return GL_MIRRORED_REPEAT;
		break;

		case WrapMode::REPAT:
			return GL_REPEAT;
		break;

		default:
			ULOGE("Can't conver this wrap type");
            return 0;
        break;
		}
	}

	constexpr GLenum SamplerConverter(Sampling sampling)
	{
		switch (sampling)
		{
		case Sampling::LINEAR:
			return GL_LINEAR;
		break;

		case Sampling::NEAREST:
			return GL_NEAREST;
		break;
		
		default:
			ULOGE("Can't conver this sampling type");
			return 0;
		break;
		}
	}
}

Texture2DOpenGL::Texture2DOpenGL()
{
	glGenTextures(1, &m_textureID);
}

Texture2DOpenGL::~Texture2DOpenGL()
{
	glDeleteTextures(1, &m_textureID);
}

void Texture2DOpenGL::SetData(glm::uvec2 size, ColorFormat InChannel, ColorFormat OutChannel, const uint8_t* data)
{
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, ConverterChannel(InChannel), size.x, size.y, 0, ConverterChannel(OutChannel), GL_UNSIGNALED, data);
	Unbind();

	m_size = size;
}

void Texture2DOpenGL::EnableMinMap(bool minMap) const
{
	Bind();
	glGenerateMipmap(GL_TEXTURE_2D);
	Unbind();
}

void Texture2DOpenGL::SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap) const
{
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConverterWrap(horizontalWrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConverterWrap(verticalWrap));
	Unbind();
}

void Texture2DOpenGL::SetBorderColor(glm::vec4 color) const
{
	Bind();
	float borderColor[4] = {color.r, color.g, color.b, color.a};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	Unbind();
}

void Texture2DOpenGL::SetSampling(Sampling magSampling, Sampling minSampling) const
{
	Bind();
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, SamplerConverter(magSampling));
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, SamplerConverter(minSampling));
	Unbind();
}

void Texture2DOpenGL::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture2DOpenGL::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}