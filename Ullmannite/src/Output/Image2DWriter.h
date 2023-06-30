#pragma once
#include <string>
#include <vector>
#include <optional>
#include "glm/vec2.hpp"

namespace Ull
{
	class Image2DWriter
	{
	public:
		enum class Format
		{
			PNG,
			BMP,
			TGA,
			JPG,
		};

		enum class Channels : uint8_t
		{
			MONO = 1u,
			MONO_A = 2u,
			RGB = 3u,
			RGBA = 4u
		};

	public:
		Image2DWriter(glm::uvec2 size, const Channels channels);

		//MAKE IT A TEMPLATE
		void SetImageData(const std::vector<uint8_t>& pixelData);
		void AddImageData(glm::uvec2 position, glm::uvec2 size, const std::vector<uint8_t>& pixelData);
		void AddImageData(glm::uvec2 position, glm::uvec2 size, const std::vector<uint16_t>& pixelData);
		void AddImageData(glm::uvec2 position, glm::uvec2 size, const std::vector<float>& pixelData, const float maxValue);

		void SaveToFile(const std::string& fileName, const Format format = Format::BMP, const std::optional<const std::string> outputPath = std::nullopt);

	private:
		const glm::uvec2 m_size;
		const Channels m_channels;

		std::vector<uint8_t> m_pixelData;
	};
}