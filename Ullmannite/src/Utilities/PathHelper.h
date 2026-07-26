#pragma once
#include <string>
#include <optional>

namespace Ull
{
	std::optional<std::wstring> ExtractDestinationFolderFromPath(const std::wstring& path);
}