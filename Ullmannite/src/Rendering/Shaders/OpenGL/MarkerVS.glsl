#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;


uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 testColor;

void main()
{
	gl_Position =   projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
	testColor = aColor;
}