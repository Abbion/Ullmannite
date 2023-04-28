#include "Ullpch.h"
#include "TextureOpenGL.h"
#include "glad/glad.h"
#include "Logger/Logger.h"
#include "DataConverterOpenGL.h"

using namespace Ull;

Texture1DOpenGL::Texture1DOpenGL()
{
    glGenTextures(1, &m_textureID);
}

Texture1DOpenGL::~Texture1DOpenGL()
{
	glDeleteTextures(1, &m_textureID);
}

void Texture1DOpenGL::SetData(std::uint16_t size, InternalDataFormat internalDataFormat, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data)
{
    Bind();
    glTexImage1D(GL_TEXTURE_1D, 0, ConverterInternalFormat(internalDataFormat), size, 0, ConverterPixelFormat(pixelDataFormat), ConvetDataType(dataType), data);
    Unbind();

    m_size = size;
}

 void Texture1DOpenGL::EnableMinMap() const
 {
    Bind();
    glGenerateMipmap(GL_TEXTURE_1D);
    Unbind();
 }
 
void Texture1DOpenGL::SetWrap(WrapMode horizontalWrap) const
{
    Bind();
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, ConverterWrap(horizontalWrap));
    Unbind();
}

void Texture1DOpenGL::SetBorderColor(glm::vec4 color) const
{
    Bind();
    float borderColor[4] = { color.r, color.g, color.b, color.a };
    glTexParameterfv(GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, borderColor);
    Unbind();
}

void Texture1DOpenGL::SetSampling(Sampling magSampling, Sampling minSampling) const
{
    Bind();
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, SamplerConverter(magSampling));
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, SamplerConverter(minSampling));
    Unbind();
}

void Texture1DOpenGL::Bind() const
{
    glBindTexture(GL_TEXTURE_1D, m_textureID);
}

void Texture1DOpenGL::Unbind() const
{
    glBindTexture(GL_TEXTURE_1D, 0);
}

void Texture1DOpenGL::BindImage(InternalDataFormat internalDataFormat, ReadWriteRights readWriteRights, std::uint8_t bindIndex) const
{
    glBindImageTexture(bindIndex, m_textureID, 0, GL_FALSE, 0, ConverterReadWriteRights(readWriteRights), ConverterInternalFormat(internalDataFormat));
}



Texture2DOpenGL::Texture2DOpenGL()
{
	glGenTextures(1, &m_textureID);
}

Texture2DOpenGL::~Texture2DOpenGL()
{
	glDeleteTextures(1, &m_textureID);
}

void Texture2DOpenGL::SetData(glm::uvec2 size, InternalDataFormat internalDataFormat, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data)
{
    //Don't do Bind, Use m_textureID instead of GL_TEXTURE_2D
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, ConverterInternalFormat(internalDataFormat), size.x, size.y, 0, ConverterPixelFormat(pixelDataFormat), ConvetDataType(dataType), data);
    Unbind();

    m_size = size;
}

void Texture2DOpenGL::SetStorage(glm::uvec2 size, InternalDataFormat internalDataFormat)
{
    glTextureStorage2D(m_textureID, 1, ConverterInternalFormat(internalDataFormat), size.x, size.y);
    m_size = size;
}

void Texture2DOpenGL::SetSubData(glm::uvec2 position, glm::uvec2 size, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data)
{
    glTextureSubImage2D(m_textureID, 0, position.x, position.y, size.x, size.y, ConverterPixelFormat(pixelDataFormat), ConvetDataType(dataType), data);
}

void Texture2DOpenGL::EnableMinMap() const
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

void Texture2DOpenGL::BindImage(InternalDataFormat internalDataFormat, ReadWriteRights readWriteRights, std::uint8_t bindIndex) const
{
    glBindImageTexture(bindIndex, m_textureID, 0, GL_FALSE, 0, ConverterReadWriteRights(readWriteRights), ConverterInternalFormat(internalDataFormat));
}



Texture3DOpenGL::Texture3DOpenGL()
{
    glGenTextures(1, &m_textureID);
}

Texture3DOpenGL::~Texture3DOpenGL()
{
    glDeleteTextures(1, &m_textureID);
}

void Texture3DOpenGL::SetData(glm::uvec3 size, InternalDataFormat internalDataFormat, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data)
{
    Bind();
    glTexImage3D(GL_TEXTURE_3D, 0, ConverterInternalFormat(internalDataFormat), size.x, size.y, size.z, 0, ConverterPixelFormat(pixelDataFormat), ConvetDataType(dataType), data);
    Unbind();

    m_size = size;
}

void Texture3DOpenGL::EnableMinMap() const
{
    Bind();
    glGenerateMipmap(GL_TEXTURE_3D);
    Unbind();
}

void Texture3DOpenGL::SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap, WrapMode depthWrap) const
{
    Bind();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, ConverterWrap(horizontalWrap));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, ConverterWrap(verticalWrap));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, ConverterWrap(depthWrap));
    Unbind();
}

void Texture3DOpenGL::SetBorderColor(glm::vec4 color) const
{
    Bind();
    float borderColor[4] = { color.r, color.g, color.b, color.a };
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, borderColor);
    Unbind();
}

void Texture3DOpenGL::SetSampling(Sampling magSampling, Sampling minSampling) const
{
    Bind();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, SamplerConverter(magSampling));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, SamplerConverter(minSampling));
    Unbind();
}

void Texture3DOpenGL::Bind() const
{
    glBindTexture(GL_TEXTURE_3D, m_textureID);
}

void Texture3DOpenGL::Unbind() const
{
    glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3DOpenGL::BindImage(InternalDataFormat internalDataFormat, ReadWriteRights readWriteRights, std::uint8_t bindIndex) const
{
    glBindImageTexture(bindIndex, m_textureID, 0, GL_TRUE, 0, ConverterReadWriteRights(readWriteRights), ConverterInternalFormat(internalDataFormat));
}