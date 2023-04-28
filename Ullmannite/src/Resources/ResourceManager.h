#pragma once
#include "Ullpch.h"
#include "Fonts/FontManager.h"

namespace Ull
{
	class ResourceManager
	{
	public:
		NON_COPYABLE(ResourceManager);

		~ResourceManager();

		static ResourceManager& GetInstance();
		void Init();
		void Terminate();

		FontManager& GetFontMnager()				{ return m_fontManager; }
		const FontManager& GetFontMnager() const	{ return m_fontManager; }

	protected:
		ResourceManager() = default;

		static ResourceManager m_resourceManager;

	private:
		FontManager m_fontManager;
	};
}