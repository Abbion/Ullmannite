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
#include "Utilities/NonCopyable.h"
#include "Logger/Logger.h"

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#define NON_COPYABLE(className) className(const className&) = delete; \
								className& operator=(className&) = delete; \
