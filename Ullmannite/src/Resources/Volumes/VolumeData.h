#pragma once
#include <vector>
#include <string>

namespace Ull
{
	struct VolumeDataI
	{
		std::wstring name{ L"" };

		uint16_t width{ 0 };
		uint16_t height{ 0 };
		uint16_t depth{ 0 };

		int16_t maxValue{ 0 };
		int16_t minValue{ 0 };

		std::vector<int16_t> dataBuffer{};
	};

	struct VolumeData
	{
		std::wstring name{ L"" };

		uint16_t width{ 0 };
		uint16_t height{ 0 };
		uint16_t depth{ 0 };

		uint16_t maxValue{ 0 };
		uint16_t minValue{ 0 };

		std::vector<uint16_t> dataBuffer{};
	};
}