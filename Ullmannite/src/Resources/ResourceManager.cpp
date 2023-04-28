#include "Ullpch.h"
#include "ResourceManager.h"

using namespace Ull;

ResourceManager ResourceManager::m_resourceManager;

ResourceManager::~ResourceManager()
{

}

ResourceManager& ResourceManager::GetInstance() 
{
	return m_resourceManager;
}

void ResourceManager::Init()
{
	
}

void ResourceManager::Terminate()
{
	m_fontManager.UnloadAllFonts();
}
