#pragma once
#include "Ullpch.h"
#include "GraphicsTypes.h"

namespace Ull
{
    class Shader
    {
    public:
        NON_COPYABLE(Shader);

        virtual ~Shader();

        static Shader* Create(const std::string& vertexShaderName, const std::string& fragmentShaderName, const std::string& geometryShaderName);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(std::string uniformName, int value) const = 0;
        virtual void SetFloat(std::string uniformName, float value) const = 0;
        virtual void SetDouble(std::string uniformName, double value) const const = 0;
        virtual void SetBool(std::string uniformName, bool value) const = 0;

    protected:
        Shader() = default;
    };
}
