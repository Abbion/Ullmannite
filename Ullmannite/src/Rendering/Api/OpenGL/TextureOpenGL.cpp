#include "Ullpch.h"
#include "TextureOpenGL.h"
#include "glad/glad.h"
#include "Logger/Logger.h"
#include "DataConverterOpenGL.h"

using namespace Ull;

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
	glTexImage2D(GL_TEXTURE_2D, 0, ConverterChannel(InChannel), size.x, size.y, 0, ConverterChannel(OutChannel), GL_UNSIGNED_BYTE, data);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, SamplerConverter(magSampling));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, SamplerConverter(minSampling));
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