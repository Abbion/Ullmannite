#include "Ullpch.h"
#include "ShaderOpenGL.h"
#include "Logger/Logger.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>
#include <string>

using namespace Ull;
#define OPENGL_SHADDER_PATH "src/Rendering/Shaders/OpenGL/"
#define OPENGL_SHADER_EXTENSION ".glsl"

namespace
{
    const char * ShaderTypeToString(GraphicsShaderType type)
    {
        switch (type)
        {
        case GraphicsShaderType::VERTEX:
            return "Vertex";
        break;
        case GraphicsShaderType::FRAGMENT:
            return "Fragment";
        break;
        case GraphicsShaderType::GEOMETRY:
            return "Geometry";
        break;
        case GraphicsShaderType::COMPUTE:
            return "Compute";
        break;        
        default:
            return "Unknown shader type";
        break;
        }
    }
}

ShaderOpenGL::ShaderOpenGL(const std::string& vertexShaderName, const std::string& fragmentShaderName, const std::string& geometryShaderName)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string vertexShaderPath = OPENGL_SHADDER_PATH + vertexShaderName + OPENGL_SHADER_EXTENSION;
    std::string fragmentShaderPath = OPENGL_SHADDER_PATH + fragmentShaderName + OPENGL_SHADER_EXTENSION;
    std::string geometryShaderPath = OPENGL_SHADDER_PATH + geometryShaderName + OPENGL_SHADER_EXTENSION;

    const bool geometryShaderSet = geometryShaderName.empty() ? false : true;

    try
    {
        vShaderFile.open(vertexShaderPath.c_str());
        fShaderFile.open(fragmentShaderPath.c_str());
        
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if(geometryShaderSet)
        {
            gShaderFile.open(geometryShaderPath.c_str());

            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();

            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch(const std::exception& e)
    {
        UASSERT(false, "Shader file cannot be loaded: " << e.what());
    }

    unsigned int vertex, fragment, geometry;
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, GraphicsShaderType::VERTEX);
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, GraphicsShaderType::FRAGMENT);

    if (geometryShaderSet)
    {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        CheckCompileErrors(geometry, GraphicsShaderType::GEOMETRY);
    }
    
    m_shaderID = glCreateProgram();
    glAttachShader(m_shaderID, vertex);
    glAttachShader(m_shaderID, fragment);

    if(geometryShaderSet)
        glAttachShader(m_shaderID, geometry);
    
    glLinkProgram(m_shaderID);
    CheckCompileErrors(m_shaderID, GraphicsShaderType::PROGRAM);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if(geometryShaderSet)
        glDeleteShader(geometry);
}

ShaderOpenGL::~ShaderOpenGL()
{
    glDeleteProgram(m_shaderID);
}

void ShaderOpenGL::Bind() const
{
    glUseProgram(m_shaderID);
}

void ShaderOpenGL::Unbind() const
{
    glUseProgram(0);
}

void ShaderOpenGL::SetInt(std::string uniformName, int value) const
{
    glUniform1i(glGetUniformLocation(m_shaderID, uniformName.c_str()), value);
}

void ShaderOpenGL::SetFloat(std::string uniformName, float value) const
{
    glUniform1f(glGetUniformLocation(m_shaderID, uniformName.c_str()), value);
}

void ShaderOpenGL::SetFloat4(std::string uniformName, glm::vec4 value) const
{
    glUniform4f(glGetUniformLocation(m_shaderID, uniformName.c_str()), value.r, value.g, value.b, value.a);
}

void ShaderOpenGL::SetFloat4x4(std::string uniformName, glm::mat4x4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderOpenGL::SetDouble(std::string uniformName, double value) const
{
    glUniform1d(glGetUniformLocation(m_shaderID, uniformName.c_str()), value);
}

void ShaderOpenGL::SetBool(std::string uniformName, bool value) const
{
    glUniform1i(glGetUniformLocation(m_shaderID, uniformName.c_str()), (int)value);
}

void ShaderOpenGL::CheckCompileErrors(unsigned int shader, GraphicsShaderType type)
{
    int success;
    char infoLog[1024];

    if(type != GraphicsShaderType::PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            ULOGE("Shader compilation failed: " << ShaderTypeToString(type) << "\n" << infoLog << "\n-----");
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            ULOGE("Program linking failed:\n" << infoLog << "\n-----");
        }
    }
}