#pragma once
#include <glm/glm.hpp>

namespace Ull
{
	typedef std::function<void(glm::vec4)> onColorChangeCallback;

	struct ColorPickerData
	{
		glm::vec4 initialColor;
		onColorChangeCallback onColorChange;
	};
}