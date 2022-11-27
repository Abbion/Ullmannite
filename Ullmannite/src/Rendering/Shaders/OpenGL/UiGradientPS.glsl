#version 460 core

out vec4 FragColor;

in VS_OUT{
    float texCoord;
} vs_out;

uniform sampler1D transferTexture;

void main()
{
	vec3 fianlColor = texture(transferTexture, vs_out.texCoord).rgb;
	FragColor = vec4(fianlColor, 1.0);
}