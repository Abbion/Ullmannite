#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCord;
uniform mat4 modelMatrix;

out VS_OUT{
    float texCoord;
} vs_out;

void main()
{
	gl_Position = modelMatrix * vec4(aPos, 1.0);
	vs_out.texCoord = aTexCord;
}