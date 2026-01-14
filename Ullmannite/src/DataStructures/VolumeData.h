#pragma once
#include <vector>

namespace Ull
{
	struct VolumeData
	{
		uint16_t width;
		uint16_t height;
		uint16_t depth;

		int16_t maxValue;
		int16_t minValue;

		std::vector<int16_t> dataBuffer;
	};
}