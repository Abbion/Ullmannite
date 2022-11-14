#pragma once
#include "DataStructures/VolumeData.h"

namespace Ull
{
	std::shared_ptr<VolumeData> LoadVolumeData(const std::string filePath);
}