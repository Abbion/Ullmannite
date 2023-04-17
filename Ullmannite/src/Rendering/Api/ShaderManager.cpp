#include "Ullpch.h"
#include "ShaderManager.h"
#include "Logger/Logger.h"
#include "Renderer.h"
#include "OpenGL/ShaderOpenGL.h"

using namespace Ull;

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	for (auto& shaderPair : m_shaderMap)
	{
		UASSERT(shaderPair.second.use_count() == 2l, "This shader is still in use");
	}

	m_shaderMap.clear();
}

std::shared_ptr<Shader> ShaderManager::GetShader(const ShaderTag tag)
{
	UASSERT(IsShaderLoaded(tag), "Can't get shader with current tag");
	return m_shaderMap[tag];
}

void ShaderManager::LoadShader(const ShaderTag tag, const std::string vertexShaderName, const std::string pixelShaderName, const std::string geometryShaderName)
{
	if (IsShaderLoaded(tag))
	{
		ULOGE("Shader " << vertexShaderName << ", " << pixelShaderName << " already loaded");
		return;
	}

	switch (Renderer::GetInstance().GetApi())
	{
	case Renderer::API::OPEN_GL:
		m_shaderMap.insert(std::make_pair<const ShaderTag, std::shared_ptr<Shader>>(
			std::move(tag),
			std::move(std::make_shared<ShaderOpenGL>(vertexShaderName, pixelShaderName, geometryShaderName)))
		);
		break;

	default:
		ULOGE("Current API didn't implement compute shader");
		break;
	} 
}

void ShaderManager::LoadShader(const ShaderTag tag, const std::string computeShaderName)
{
	if (IsShaderLoaded(tag))
	{
		ULOGE("Compute shader " << computeShaderName << " already loaded");
		return;
	}

	switch (Renderer::GetInstance().GetApi())
	{
	case Renderer::API::OPEN_GL:
		m_shaderMap.insert(std::make_pair<const ShaderTag, std::shared_ptr<Shader>>(
			std::move(tag),
			std::move(std::make_shared<ShaderOpenGL>(computeShaderName)))
		);
		break;

	default:
		ULOGE("Current API didn't implement compute shader");
		break;
	}
}

void ShaderManager::UnloadShader(const ShaderTag tag)
{
	UASSERT(IsShaderLoaded(tag), "Can't delete shader with current tag");
	m_shaderMap.erase(tag);
}

bool ShaderManager::IsShaderLoaded(const ShaderTag tag)
{
	return m_shaderMap.find(tag) == m_shaderMap.end() ? false : true;
}
