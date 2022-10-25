#pragma once
#include<cstdint>
#include "Rendering/Api/Texture.h"

namespace Ull
{
	class TriangulationTable
	{
	public:
		~TriangulationTable();

		static TriangulationTable& GetInstance();
		
		Texture2D* GetTriangulationTexture() { return m_triangulationTexture; }

		void CreateTriangulationTable();

	protected:
		TriangulationTable() = default;

		static TriangulationTable m_instance;

		inline static Texture2D* m_triangulationTexture{ nullptr };
	};
}