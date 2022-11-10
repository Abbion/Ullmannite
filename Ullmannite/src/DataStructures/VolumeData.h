#pragma once
#include <vector>

namespace Ull
{
	struct VolumeData
	{
		VolumeData(size_t dataBufferSize) : dataBuffer(dataBufferSize), width(1), height(1), depth(1)
		{}

		VolumeData(uint16_t width, uint16_t height, uint16_t depth, size_t dataBufferSize) : width(width), height(height), depth(depth), dataBuffer(dataBufferSize)
		{}

		uint16_t width;
		uint16_t height;
		uint16_t depth;
		uint16_t maxValue;

		std::vector<int16_t> dataBuffer;
	};
}