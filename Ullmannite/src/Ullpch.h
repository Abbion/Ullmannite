#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <algorithm>

#include <string>
#include <vector>
#include <map>
#include <format>

#include <glm/glm.hpp>

#include "Rendering/Utils/RenderHelper.h"
#include "Exceptions/Exceptions.h"
#include "Utilities/PointerHelper.h"
#include "Utilities/FloatHelper.h"
#include "Utilities/NonCopyable.h"
#include "Logger/Logger.h"
#include "Core/Constants.h"
#include "Core/Types.h"

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#define NON_COPYABLE(className) className(const className&) = delete; \
								className& operator=(className&) = delete; \