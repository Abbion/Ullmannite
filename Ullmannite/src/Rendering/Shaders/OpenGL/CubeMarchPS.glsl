#version 460 core

out vec4 FragColor;
in vec3 colorTA;

in float intensity;
in vec3 normal;

uniform sampler1D transferTexture;

void main()
{
	vec3 lightDir = vec3(0.0, 1.0, 1.0);
	lightDir = normalize(lightDir);

	//FragColor = texture(transferTexture, intensity);
	FragColor = vec4(normal * dot(normal, lightDir), 1.0);
}