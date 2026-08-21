#include "Ullpch.h"
#include "ResourceManager.h"

using namespace Ull;

ResourceManager::~ResourceManager()
{
	m_fontManager.UnloadAllFonts();
}
