#version 330 core

out vec4 FragColor;
uniform vec3 materialColor;
in vec3 colorT;

void main()
{
	FragColor = vec4(colorT, 1.0);
}