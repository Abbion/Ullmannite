#pragma once
#include <array>
#include <glm/glm.hpp>

namespace Ull
{
	struct CuttingSettings
	{
		glm::vec3 cuttingPositions;
		std::array<bool, 3> invertedAxis;

		bool operator==(const CuttingSettings& cuttingSetting)
		{
			return (this->cuttingPositions == cuttingSetting.cuttingPositions && this->invertedAxis == cuttingSetting.invertedAxis);
		}

		bool operator!=(const CuttingSettings& cuttingSetting)
		{
			return !((*this) == cuttingSetting);
		}
	};
}