#version 460 core

layout(rgba32f, binding = 0) readonly uniform image3D vertexPosTexture;

uniform uvec3 vertexPosTextureSize;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out VS_OUT{
    vec3 colorT;
} vs_out;

//out float intensity;

void main()
{
    ivec3 vertexPosition = ivec3(gl_VertexID % 2048, (gl_VertexID / 2048) % 2048, (gl_VertexID / (2048 * 2048)));
    vec4 vertexData = imageLoad(vertexPosTexture, vertexPosition);

    vs_out.colorT = vec3(vertexData.xyz);
    //intensity = vertexData.w;

	gl_Position = projectionMatrix * viewMatrix * vec4(vertexData.xyz, 1.0);
}