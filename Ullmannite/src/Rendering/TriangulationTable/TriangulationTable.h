#pragma once
#include<cstdint>
#include <array>
#include "Rendering/Api/Texture.h"

namespace Ull
{
	class TriangulationTable
	{
	public:
		~TriangulationTable();

		static TriangulationTable& GetInstance();
		
		const std::array<uint8_t, 256>& GetVertexCountTable() { return m_vertexCountTable; }
		Texture2D* GetTriangulationTexture() { return m_triangulationTexture; }
		Texture1D* GetVertexCountTexture() { return m_vertexCountTexture; }

		void CreateTriangulationTable();
		void CreateVectexCountTable();

	protected:
		TriangulationTable() = default;
		static void InitVertexCountTable();

		static TriangulationTable m_instance;

		static std::array<uint8_t, 256> m_vertexCountTable;
		inline static bool tableInit{ false };
		inline static Texture2D* m_triangulationTexture{ nullptr };
		inline static Texture1D* m_vertexCountTexture{ nullptr };
	};
}