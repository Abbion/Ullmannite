#pragma once
#include "Rendering/Api/Shader.h"
#include "Rendering/Api/GraphicsTypes.h"

namespace Ull
{
    class ShaderOpenGL : public Shader
    {
    public:
        ShaderOpenGL(const std::string& vertexShaderName, const std::string& fragmentShaderName, const std::string& geometryShaderName);
        ~ShaderOpenGL();

        void Bind() const override;
        void Unbind() const override;

        void SetInt(std::string uniformName, int value) const override;
        void SetFloat(std::string uniformName, float value) const override;
        void SetFloat4(std::string uniformName, glm::vec4 value) const override;
        void SetDouble(std::string uniformName, double value) const override;
        void SetBool(std::string uniformName, bool value) const override;

    private:
        void CheckCompileErrors(unsigned int shader, GraphicsShaderType type);

        uint32_t m_shaderID;
    };
}