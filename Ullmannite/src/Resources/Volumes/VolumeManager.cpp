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
	
	const auto data = LoadVolumeData(folderPath);

	if (data.width == 0 || data.height == 0 || data.depth == 0 || data.dataBuffer.size() == 0)
	{
		m_data = VolumeData{};
		return false;
	}

	m_data = std::move(ConvertToUnsignedRange(data));
	m_data.name = folderName.value();
	return true;
}

void VolumeManager::CreateTestData()
{
	auto data = VolumeDataI{};
	data.width = 3;
	data.height = 3;
	data.depth = 3;
	data.name = L"Test 3x3";
	data.minValue = -50;
	data.maxValue = 50;
	data.dataBuffer = {	-50, -50, -50,
						-50, -50, -50,
						-50, -50, -50,
		
						-50, -50, -50,
						-50, +50, +25,
						-50, -50, -50,
		
						-50, -50, -50,
						-50, -50, -50, 
						-50, -50, -50, };

	m_data = ConvertToUnsignedRange(data);
}

VolumeData VolumeManager::ConvertToUnsignedRange(const VolumeDataI& input)
{
	VolumeData output;

	output.width = input.width;
	output.height = input.height;
	output.depth = input.depth;

	output.minValue = 0;
	output.maxValue = input.maxValue - input.minValue;

	output.dataBuffer.reserve(input.dataBuffer.size());

	for (auto voxel : input.dataBuffer)
		output.dataBuffer.push_back(static_cast<uint16_t>(voxel - input.minValue));
	
	return output;
}
