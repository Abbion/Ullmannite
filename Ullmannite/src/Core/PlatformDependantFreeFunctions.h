#pragma once
#include <optional>

namespace Ull
{
	enum class FileExtentions
	{
		DAT = 1 << 0,
		DICOM = 1 << 1
	};

	std::optional<std::wstring> CreateFileOpenDialog(FileExtentions fileExtentions);
	void CreateDataFromDicom(std::wstring filePath);

	inline FileExtentions operator|(FileExtentions flagA, FileExtentions flagB)
	{
		return static_cast<FileExtentions>(static_cast<uint64_t>(flagA) | static_cast<uint64_t>(flagB));
	}

	inline bool operator&(FileExtentions flagA, FileExtentions flagB)
	{
		return static_cast<bool>(static_cast<uint64_t>(flagA) & static_cast<uint64_t>(flagB));
	}
}