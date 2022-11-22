#pragma once
#include <glm/glm.hpp>

namespace Ull
{
	template<typename vec2T>
	bool PointInStaticRect(vec2T point, vec2T rectPosition, vec2T rectSize)	//Static -> non rotated
	{
		auto pontToRectPos = point - rectPosition;

		if (pontToRectPos.x < 0 || pontToRectPos.y < 0)
			return false;

		if (pontToRectPos.x <= rectSize.x && pontToRectPos.y <= rectSize.y)
			return true;

		return false;
	}

	template<typename vec2T>
	bool PointInTriangle(vec2T point, vec2T p1, vec2T p2, vec2T p3)
	{
		//TODO: Fill this
	}
}