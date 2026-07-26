#pragma once
#include "Ullpch.h"
#include "Fonts/FontManager.h"
#include "Volumes/VolumeManager.h"

namespace Ull
{
	class ResourceManager
	{
	public:
		NON_COPYABLE(ResourceManager);

		~ResourceManager();

		ResourceManager& GetInstance();

		FontManager& GetFontMnager()					{ return m_fontManager; }
		const FontManager& GetFontMnager() const		{ return m_fontManager; }

		VolumeManager& GetVolumeManager()				{ return m_volumeManager; }
		const VolumeManager& GetVolumeManager() const	{ return m_volumeManager; }

	protected:
		ResourceManager() = default;

	private:

		FontManager m_fontManager;
		VolumeManager m_volumeManager;
		friend class Application;
	};
}