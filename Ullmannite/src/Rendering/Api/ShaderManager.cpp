#include "Ullpch.h"
#include "ShaderManager.h"
#include "Logger/Logger.h"

using namespace Ull;

ShaderManager ShaderManager::m_shaderManagerInstance;

ShaderManager::ShaderManager()
{
	m_shaderMap[ShaderTag::UI_SHADER] = nullptr;
}

ShaderManager::~ShaderManager()
{
	for (auto shaderPair : m_shaderMap)
		delete shaderPair.second;
	m_shaderMap.clear();
}

ShaderManager& ShaderManager::GetInstance()
{
	return m_shaderManagerInstance;
}

Shader* ShaderManager::GetShader(ShaderTag tag)
{
	return m_shaderMap[tag];
}

void ShaderManager::LoadShader(ShaderTag tag, std::string vertexShaderName, std::string pixelShaderName, std::string geometryShaderName)
{
	if (m_shaderMap[tag] != nullptr)
	{
		ULOGE("Shader " << vertexShaderName << ", " << pixelShaderName << " already loaded");
		return;
	}

	m_shaderMap[tag] = Shader::CreateRenderShader(vertexShaderName.c_str(), pixelShaderName.c_str(), geometryShaderName.empty() ? nullptr : geometryShaderName.c_str());
}

void ShaderManager::LoadShader(ShaderTag tag, std::string computeShaderName)
{
	if (m_shaderMap[tag] != nullptr)
	{
		ULOGE("Compute shader " << computeShaderName << " already loaded");
		return;
	}

	m_shaderMap[tag] = Shader::CreateComputeShader(computeShaderName.c_str());
}

void ShaderManager::UnloadShader(ShaderTag tag)
{
	delete m_shaderMap[tag];
	m_shaderMap[tag] = nullptr;
}