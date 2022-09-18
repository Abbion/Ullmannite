#pragma once
#include "Shader.h"
#include <map>

namespace Ull
{
	class ShaderManager
	{
	public:
		NON_COPYABLE(ShaderManager);
		~ShaderManager();

		static ShaderManager& GetInstance();
		void LoadShader(ShaderTag tag, std::string vertexShaderName, std::string pixelShaderName, std::string geometryShaderName = "");
		void UnloadShader(ShaderTag tag);
		Shader* GetShader(ShaderTag tag);

	protected:
		ShaderManager();
		static ShaderManager m_shaderManagerInstance;

	private:
		std::map<ShaderTag, Shader*> m_shaderMap;
	};
}