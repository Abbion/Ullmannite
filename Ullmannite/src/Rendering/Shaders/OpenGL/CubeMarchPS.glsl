#version 460 core

struct LightSettings{
	vec3 lightDir;
	float ambientStrength;
} lightSettings;

out vec4 FragColor;

in GS_OUT{
    vec3 normal;
    float intensity;
} gs_out;

uniform sampler1D transferTexture;


void main()
{
	vec3 lightDir = vec3(1.0, -1.0, 0.0);
	lightDir = normalize(lightDir);

	//FragColor = texture(transferTexture, intensity);
	FragColor = vec4(vec3(0.5, 0.5, 0.5) * max(dot(gs_out.normal, lightDir), 0.15), 1.0);
}