#pragma once
#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
	class UiColorPicker : public UiBasicControl
	{
	public:
		UiColorPicker(const std::string name, const glm::vec2 position, const glm::vec2 size);
	};
}