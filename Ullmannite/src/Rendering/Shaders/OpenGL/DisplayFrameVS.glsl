#version 420 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec2 InTextureCords;

out vec2 OutTextureCoords;
uniform mat4 viewMatrix;

void main()
{
	OutTextureCoords = InTextureCords;
	gl_Position = viewMatrix * vec4(InPosition, 1.0);
}