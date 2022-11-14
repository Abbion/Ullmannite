#version 460 core

struct LightSettings{
	vec3 lightDir;
	vec3 lightColor;
	float ambientStrength;
};

out vec4 FragColor;

in GS_OUT{
    vec3 normal;
    float intensity;
} gs_out;

uniform sampler1D transferTexture;
uniform LightSettings lightSettings;

void main()
{
	vec3 lightDir = normalize(lightSettings.lightDir);

	vec3 ambient = lightSettings.ambientStrength * lightSettings.lightColor;
	vec3 diffuse = max(dot(gs_out.normal, lightDir), 0) * lightSettings.lightColor;

	//vec3 fianlColor = (ambient + diffuse) * texture(transferTexture, gs_out.intensity).rgb;
	vec3 fianlColor = (ambient + diffuse) * vec3(0.5, 0.5, 0.5);

	FragColor = vec4(fianlColor, 1.0);
}