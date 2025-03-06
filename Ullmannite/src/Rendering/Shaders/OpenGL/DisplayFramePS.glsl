#version 460 core
out vec4 OutFragmentColor;

in vec2 TextureCoords;

uniform sampler2D frameTexture;

void main()
{
	OutFragmentColor = texture(frameTexture, TextureCoords);
}
