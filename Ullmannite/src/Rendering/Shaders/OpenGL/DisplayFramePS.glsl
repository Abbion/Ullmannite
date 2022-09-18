#version 420 core
out vec4 OutFragmentColor;

in vec2 TextureCoords;

uniform sampler2D frameTexture;

void main()
{
	OutFragmentColor = vec4(texture(frameTexture, TextureCoords).rgb, 1.0);
}
