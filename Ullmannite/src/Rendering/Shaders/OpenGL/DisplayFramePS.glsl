#version 420 core
out vec4 OutFragmentColor;

in vec2 InTextureCoords;

uniform sampler2D frameTexture;

void main()
{
	OutFragmentColor = vec4(texture(frameTexture, InTextureCoords).rgb, 1.0);
}