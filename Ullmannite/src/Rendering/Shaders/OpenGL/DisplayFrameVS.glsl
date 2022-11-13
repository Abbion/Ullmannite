#version 460 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec2 InTextureCords;

out vec2 TextureCoords;
uniform mat4 viewMatrix;

void main()
{
	TextureCoords = InTextureCords;
	gl_Position = viewMatrix * vec4(InPosition, 1.0);
}
