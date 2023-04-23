#pragma once
#include "Shader.h"
#include <map>

namespace Ull
{
	class ShaderManager
	{
	public:
		NON_COPYABLE(ShaderManager);
		ShaderManager();
		~ShaderManager();

		std::shared_ptr<Shader> GetShader(ShaderTag tag);

		void LoadShader(const ShaderTag tag, const std::string vertexShaderName, const std::string pixelShaderName, const std::string geometryShaderName = "");
		void LoadShader(const ShaderTag tag, const std::string computeShaderName);
		void UnloadShader(const ShaderTag tag);
		void UnloadAllShaders();

	private:
		std::map<const ShaderTag, std::shared_ptr<Shader>> m_shaderMap;

		bool IsShaderLoaded(const ShaderTag tag);
	};
}