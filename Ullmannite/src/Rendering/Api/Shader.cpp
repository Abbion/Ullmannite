#include "Ullpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "OpenGL/ShaderOpenGL.h"
#include "Logger/Logger.h"
#include <string>

using namespace Ull;

Shader* Shader::CreateRenderShader(const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName)
{
    std::string vertexShaderNameStr(vertexShaderName);
    std::string fragmentShaderNameStr(fragmentShaderName);
    std::string geometryShaderNameStr = geometryShaderName == nullptr ? std::string("") : std::string(geometryShaderName);

    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new ShaderOpenGL(vertexShaderNameStr, fragmentShaderNameStr, geometryShaderNameStr);
        break;
    }

    ULOGE("Current API didn't implement shader");
    return nullptr;
}

Shader* Shader::CreateComputeShader(const char* computeShaderName)
{
    std::string computeShaderNameStr(computeShaderName);

    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new ShaderOpenGL(computeShaderNameStr);
        break;
    }

    ULOGE("Current API didn't implement compute shader");
    return nullptr;
}