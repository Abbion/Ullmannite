#version 460 core

out vec4 FragColor;

in VS_OUT{
    vec4 color;
} vs_out;

void main()
{
	FragColor = vs_out.color;
}