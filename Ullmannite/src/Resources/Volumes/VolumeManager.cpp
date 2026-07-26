#include "Ullpch.h"
#include "VolumeManager.h"
#include "DataLoaders/VolumeLoader.h"

using namespace Ull;

VolumeData& VolumeManager::GetVolume()
{
	const auto gurad = std::lock_guard(m_mutex);
	return m_data;
}

bool VolumeManager::LoadVolumeFromFolder(const std::wstring& folderPath)
{
	const auto gurad = std::lock_guard(m_mutex);

	const auto folderName = ExtractDestinationFolderFromPath(folderPath);
	if (folderName.has_value() == false)
	{
		m_data = VolumeData{};
		return false;
	}
	
	m_data = std::move(LoadVolumeData(folderPath));

	if (m_data.width == 0 || m_data.height == 0 || m_data.depth == 0 || m_data.dataBuffer.size() == 0)
	{
		m_data = VolumeData{};
		return false;
	}

	m_data.name = folderName.value();
	return true;
}