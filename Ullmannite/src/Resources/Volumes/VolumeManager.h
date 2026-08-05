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
		void CreateTestData();

	private:
		VolumeManager() = default;
		VolumeData ConvertToUnsignedRange(const VolumeDataI& input);

		VolumeData m_data;
		std::mutex m_mutex;

		friend class ResourceManager;
	};
}