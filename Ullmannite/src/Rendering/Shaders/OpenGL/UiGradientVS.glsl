#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
uniform mat4 modelMatrix;

out VS_OUT{
    vec4 color;
} vs_out;

void main()
{
	gl_Position = modelMatrix * vec4(inPosition, 1.0);
	vs_out.color = inColor;
}