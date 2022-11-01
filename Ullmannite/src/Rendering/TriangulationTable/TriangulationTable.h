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
		Texture1D* GetVertexCountTexture() { return m_vertexCountTexture; }

		void CreateTriangulationTable();
		void CreateVectexCountTable();

	protected:
		TriangulationTable() = default;

		static TriangulationTable m_instance;

		inline static Texture2D* m_triangulationTexture{ nullptr };
		inline static Texture1D* m_vertexCountTexture{ nullptr };
	};
}