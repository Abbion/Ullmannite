#pragma once
#include "GraphicsTypes.h"

namespace Ull
{
    enum class ShaderTag {
        UI_BASIC_COLOR,
        UI_GRADIENT_SHADER,
        FRAME_DISPLAY_SHADER,
        MARKER,
        CUBE_MARCH_MESH_GENERATOR,
        TRANSFER_FUNCTION_GENERATOR,
        CUBE_MARCH_VERTEX_COUNTER,
        CUBE_MARCH_VERTEX_RENDERER,
        SIGNED_DISTANCE_FIELD_FOR_2D_TEXTURES,
        CHANGE_VALUE_IF_GREATHER_THAN_UIIMAGE_2D, //NOT USED?
        INVERSE_2D_BIT_MAP,
        MEREGE_INNSER_OUTER_SDF,
    };

    enum class ShaderType {
        RENDER,
        COMPUTE
    };

    class Shader
    {
    public:
        virtual ~Shader() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(std::string uniformName, int value) const = 0;
        virtual void SetUint(std::string uniformName, unsigned int value) const = 0;
        virtual void SetInt3(std::string uniformName, glm::ivec3 value) const = 0;
        virtual void SetInt2(std::string uniformName, glm::ivec2 value) const = 0;
        virtual void SetUint3(std::string uniformName, glm::uvec3 value) const = 0;
        virtual void SetFloat(std::string uniformName, float value) const = 0;
        virtual void SetFloat4(std::string uniformName, glm::vec4 value) const = 0;
        virtual void SetFloat3(std::string uniformName, glm::vec3 value) const = 0;
        virtual void SetFloat4x4(std::string uniformName, glm::mat4x4 value) const = 0;
        virtual void SetDouble(std::string uniformName, double value) const = 0;
        virtual void SetBool(std::string uniformName, bool value) const = 0;

        std::string GetShaderName() { return m_shaderName; }

    protected:
        Shader(const std::string& shaderName);
        ShaderType m_shaderType{ ShaderType::RENDER };

    private:
        const std::string m_shaderName;
    };
}
