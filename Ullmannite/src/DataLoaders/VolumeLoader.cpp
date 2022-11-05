#include "Ullpch.h"
#include "VolumeLoader.h"
#include "Logger/Logger.h"
#include <fstream>
#include <sstream>
#include <string>
#include <thread>

using namespace Ull;

namespace 
{
	constexpr size_t readThreads = 8;
}

std::shared_ptr<VolumeData> Ull::LoadVolumeData(const std::string filePath)
{
	std::ifstream volumeFile;
	std::shared_ptr<VolumeData> volumeData = nullptr;

	volumeFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		volumeFile.open(filePath.c_str(), std::ios::binary);

		uint16_t width, height, depth;

		volumeFile.read((char*)&width, sizeof(uint16_t));
		volumeFile.read((char*)&height, sizeof(uint16_t));
		volumeFile.read((char*)&depth, sizeof(uint16_t));

		size_t bufferSize = (size_t)width * (size_t)height * (size_t)depth;
		volumeData = std::make_shared<VolumeData>(VolumeData(width, height, depth, bufferSize));

		size_t chunkSize = bufferSize / (readThreads - 1);
		size_t lastChunkSize = bufferSize % (readThreads - 1);
		size_t dataStartPosition = (size_t)volumeFile.tellg();

		volumeFile.close();

		//=============================

		std::thread* readingThreads[readThreads];

		for (size_t i = 0; i < readThreads; ++i)
		{
			readingThreads[i] = new std::thread([&volumeData, filePath, i, dataStartPosition, chunkSize, lastChunkSize](){
				if (chunkSize == 0)
					return;

				std::ifstream volumeFile(filePath, std::ios::binary);

				UASSERT(volumeFile.good(), "Can't open file");

				volumeFile.seekg(dataStartPosition + (i * chunkSize * sizeof(uint16_t)), std::ios::beg);

				if (i == readThreads - 1)
				{
					if (lastChunkSize < 1)
						return;

					volumeFile.read((char*)&volumeData->dataBuffer[i * chunkSize], sizeof(uint16_t) * lastChunkSize);
				}
				else
					volumeFile.read((char*)&volumeData->dataBuffer[i * chunkSize], sizeof(uint16_t) * chunkSize);

				volumeFile.close();
			});
		}

		for (size_t i = 0; i < readThreads; ++i)
		{
			readingThreads[i]->join();
			delete readingThreads[i];
		}
	}
	catch (const std::exception& e)
	{
		ULOGF("File loading error: " << e.what());
	}

	return volumeData;
}
