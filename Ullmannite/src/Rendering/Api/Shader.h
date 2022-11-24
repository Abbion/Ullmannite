#pragma once
#include "GraphicsTypes.h"

namespace Ull
{
    enum class ShaderTag {
        UI_SHADER_COLOR,
        UI_GRADIENT_SHADER,
        FRAME_DISPLAY_SHADER,
        MARKER,
        CUBE_MARCH_MESH_GENERATOR,
        TRANSFER_FUNCTION_GENERATOR,
        CUBE_MARCH_VERTEX_COUNTER,
        CUBE_MARCH_VERTEX_RENDERER
    };

    enum class ShaderType {
        RENDER,
        COMPUTE
    };

    class Shader
    {
    public:
        virtual ~Shader() {}

        static Shader* CreateRenderShader(const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName = nullptr);
        static Shader* CreateComputeShader(const char* computeShaderName);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(std::string uniformName, int value) const = 0;
        virtual void SetUint(std::string uniformName, unsigned int value) const = 0;
        virtual void SetInt3(std::string uniformName, glm::ivec3 value) const = 0;
        virtual void SetUint3(std::string uniformName, glm::uvec3 value) const = 0;
        virtual void SetFloat(std::string uniformName, float value) const = 0;
        virtual void SetFloat4(std::string uniformName, glm::vec4 value) const = 0;
        virtual void SetFloat3(std::string uniformName, glm::vec3 value) const = 0;
        virtual void SetFloat4x4(std::string uniformName, glm::mat4x4 value) const = 0;
        virtual void SetDouble(std::string uniformName, double value) const = 0;
        virtual void SetBool(std::string uniformName, bool value) const = 0;

    protected:
        Shader() = default;
        ShaderType m_shaderType{ ShaderType::RENDER };
    };
}
