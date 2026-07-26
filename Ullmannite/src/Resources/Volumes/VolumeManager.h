#pragma once
#include "VolumeData.h"
#include <optional>
#include <mutex>

namespace Ull
{
	class ResourceManager;

	class VolumeManager
	{
	public:
		NON_COPYABLE(VolumeManager);

		VolumeData& GetVolume();
		bool LoadVolumeFromFolder(const std::wstring& folderPath);

	private:
		VolumeManager() = default;

		VolumeData m_data;
		std::mutex m_mutex;

		friend class ResourceManager;
	};
}