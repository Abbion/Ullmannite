#version 460 core

out vec4 FragColor;
uniform vec3 materialColor;

void main()
{
	FragColor = vec4(materialColor, 1.0f);
}