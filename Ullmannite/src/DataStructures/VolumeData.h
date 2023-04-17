#pragma once
#include <vector>

namespace Ull
{
	struct VolumeData
	{
		VolumeData(size_t dataBufferSize) : dataBuffer(dataBufferSize), width{ 1u }, height{ 1u }, depth{ 1u }, maxValue{ 0u }
		{}

		VolumeData(uint16_t width, uint16_t height, uint16_t depth, size_t dataBufferSize) : dataBuffer(dataBufferSize), width{ width }, height{ height }, depth{ depth }, maxValue{ 0u }
		{}

		uint16_t width;
		uint16_t height;
		uint16_t depth;
		uint16_t maxValue;

		std::vector<int16_t> dataBuffer;
	};
}