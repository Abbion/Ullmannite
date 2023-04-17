#pragma once

#include <iostream>
#include <memory>
#include <functional>

#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "Exceptions/Exceptions.h"
#include "Utilities/PointerHelper.h"

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#define NON_COPYABLE(className) className(const className&) = delete; \
								className& operator=(className&) = delete; \


constexpr inline float powFloatHelper(const float val, unsigned rank) noexcept 
{
	float result = 1.0f;

	for (int i = 0; i < rank; ++i)
		result *= val;

	return result;
}

