#include "Ullpch.h"
#include "TextureOpenGL.h"
#include "glad/glad.h"

using namespace Ull;

namespace
{
	unsigned int ChannelConverter(ColorChannels channel)
	{
		switch (channel)
		{
		case ColorChannels::R:
		case ColorChannels::G:
		case ColorChannels::B:
			return GL_R8;
		break;

		case ColorChannels::RG:
		case ColorChannels::RB:
		case ColorChannels::GB:
			return GL_RG;
		break;

		case ColorChannels::RGB:
			return GL_RGB;
		break;

		case ColorChannels::RGBA:
			return GL_RGBA;
		break;

		default:
            //TODO ERROR HANDLING
            return 0;
        break;
		}
	}

	unsigned int WrapConverter(WrapMode wrap)
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
            //TODO ERROR HANDLING
            return 0;
        break;
		}
	}

	unsigned int SamplerConverter(Sampling sampling)
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
			//TODO Error handling
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

void Texture2DOpenGL::SetData(unsigned int width, unsigned int height, ColorChannels InChannel, ColorChannels OutChannel, const uint8_t* data) const
{
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, ChannelConverter(InChannel), width, height, 0, ChannelConverter(OutChannel), GL_UNSIGNALED, data);
	Unbind();
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapConverter(horizontalWrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapConverter(verticalWrap));
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