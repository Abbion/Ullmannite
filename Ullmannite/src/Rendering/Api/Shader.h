#pragma once
#include "Ullpch.h"
#include "GraphicsTypes.h"

namespace Ull
{
    enum class ShaderTag {
        UI_SHADER
    };

    class Shader
    {
    public:
        virtual ~Shader() {}

        static Shader* Create(const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName = nullptr);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(std::string uniformName, int value) const = 0;
        virtual void SetFloat(std::string uniformName, float value) const = 0;
        virtual void SetFloat4(std::string uniformName, glm::vec4 value) const = 0;
        virtual void SetFloat4x4(std::string uniformName, glm::mat4x4 value) const = 0;
        virtual void SetDouble(std::string uniformName, double value) const = 0;
        virtual void SetBool(std::string uniformName, bool value) const = 0;

    protected:
        Shader() = default;
    };
}
