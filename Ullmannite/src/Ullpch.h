#pragma once

#include <iostream>
#include <memory>
#include <functional>

#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "Exceptions/Exceptions.h"

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#define NON_COPYABLE(className) className(const className&) = delete; \
								className(const className&&) = delete; \
								className& operator=(className&) = delete; \
								className& operator=(className&&) = delete;