#include "Ullpch.h"
#include "Image2DWriter.h"
#include "Logger/Logger.h"
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "StbImage/stb_image_write.h"

using namespace Ull;

namespace 
{
	const std::string FormatToStringConverter(const Image2DWriter::Format format)
	{
		switch (format)
		{
		case Image2DWriter::Format::PNG:
			return ".png";
		break;
		case Image2DWriter::Format::BMP:
			return ".bmp";
		break;
		case Image2DWriter::Format::TGA:
			return ".tga";
		break;
		case Image2DWriter::Format::JPG:
			return ".jpg";
		break;
		default:
			ULOGE("Can't conver this image format!");
			return "";
			break;
		}
	}

	constexpr uint8_t max8ui = static_cast<uint8_t>((1L << 8) - 1);
	constexpr uint16_t max16ui = static_cast<uint16_t>((1L << 16) - 1);
}

Image2DWriter::Image2DWriter(glm::uvec2 size, const Channels channels) :
	m_size{ size },
	m_channels{ channels },
	m_pixelData(m_size.x * m_size.y * (uint8_t)m_channels)
{
}

void Image2DWriter::SetImageData(const std::vector<uint8_t>& pixelData)
{
	UASSERT(m_pixelData.size() >= pixelData.size(), "Copy buffer is to samll, cant coppy data!");
	m_pixelData.assign(pixelData.begin(), pixelData.end());
}

void Image2DWriter::AddImageData(glm::uvec2 position, glm::uvec2 size, const std::vector<uint8_t>& pixelData)
{
	UASSERT(m_pixelData.size() >= pixelData.size(), "Copy buffer is to samll, cant coppy data!");

	const auto topRightCornder = position + size;

	if (topRightCornder.x > m_size.x || topRightCornder.y > m_size.y)
		ULOGE("Can't fit sub image in final image");

	for (int i = 0; i < size.y; ++i)
	{
		glm::uvec2 savePosition(0u, (position.y + i) * m_size.x);
		const unsigned readPosY = (i * size.x);

		for (int j = 0; j < size.x; ++j)
		{
			savePosition.x = position.x + j;
			m_pixelData[savePosition.y + savePosition.x] = pixelData[readPosY + j];
		}
	}
}

void Image2DWriter::AddImageData(glm::uvec2 position, glm::uvec2 size, const std::vector<uint16_t>& pixelData)
{
	std::vector<uint8_t> pixelData8bit(pixelData.size());
	std::transform(pixelData.begin(), pixelData.end(), pixelData8bit.begin(), [](uint16_t value) {
		return (uint8_t)(max8ui * ((float)value / (float)max16ui));
	});

	AddImageData(position, size, pixelData8bit);
}

void Image2DWriter::AddImageData(glm::uvec2 position, glm::uvec2 size, const std::vector<float>& pixelData, const float maxValue)
{
	std::vector<uint8_t> pixelData8bit(pixelData.size());
	std::transform(pixelData.begin(), pixelData.end(), pixelData8bit.begin(), [maxValue](uint16_t value) {
		return (uint8_t)(max8ui * ((float)value / maxValue));
	});

	AddImageData(position, size, pixelData8bit);
}

void Image2DWriter::SaveToFile(const std::string& fileName, const Format format, const std::optional<const std::string> outputPath)
{
	std::string fullFileName = (outputPath.has_value() ? outputPath.value() : "") + fileName + "Out" + FormatToStringConverter(format);
	int result = 0;

	switch (format)
	{
	case Image2DWriter::Format::PNG:
		result = stbi_write_png(fullFileName.c_str(), m_size.x, m_size.y, (int)m_channels, m_pixelData.data(), 0);
		break;
	case Image2DWriter::Format::BMP:
		result = stbi_write_bmp(fullFileName.c_str(), m_size.x, m_size.y, (int)m_channels, m_pixelData.data());
		break;
	case Image2DWriter::Format::TGA:
		result = stbi_write_tga(fullFileName.c_str(), m_size.x, m_size.y, (int)m_channels, m_pixelData.data());
		break;
	case Image2DWriter::Format::JPG:
		result = stbi_write_jpg(fullFileName.c_str(), m_size.x, m_size.y, (int)m_channels, m_pixelData.data(), 50);
		break;
	default:
		ULOGE("Can't save to this image format!");
		return;
		break;
	}

	if (result == 0)
		int a = 2;

	UASSERT(result != 0, "Can't save image to file!");
}