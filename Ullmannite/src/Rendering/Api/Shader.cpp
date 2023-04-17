#include "Ullpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "OpenGL/ShaderOpenGL.h"
#include "Logger/Logger.h"
#include <string>

using namespace Ull;


Shader::Shader(const std::string& shaderName) : m_shaderName{ shaderName }
{
}
