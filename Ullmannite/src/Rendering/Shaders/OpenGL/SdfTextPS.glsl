#version 460 core
out vec4 OutFragmentColor;

in vec2 TextureCoords;

uniform float smoothing;
uniform float threshold;
uniform vec4 color;

uniform sampler2D sdfTextTexture;

void main()
{
	const float value = texture(sdfTextTexture, TextureCoords).r;
	float opacity = 1.0f;

	if (value > threshold)
		opacity = 1.0 - ((value - threshold) / smoothing);

	if (value > threshold + smoothing)
		discard;

	OutFragmentColor = vec4(color.rgb, opacity * color.a);
}
