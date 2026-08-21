#include "Ullpch.h"
#include "PathHelper.h"

std::optional<std::wstring> Ull::ExtractDestinationFolderFromPath(const std::wstring& path)
{
	const auto lastSlashPosition = path.find_last_of(L"\\");
	if (lastSlashPosition == std::wstring::npos)
		return std::nullopt;

	const auto firstCharacterPosition = lastSlashPosition + 1;
	const auto destinationNameLength = path.length() - firstCharacterPosition;

	if (destinationNameLength <= 0)
		return std::nullopt;

	return path.substr(firstCharacterPosition, destinationNameLength);
}
