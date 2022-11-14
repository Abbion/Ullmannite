#version 460 core

layout(rgba32f, binding = 0) readonly uniform image3D vertexPosTexture;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out VS_OUT{
    vec3 rawPosition;
    float intensity;
} vs_out;

const uint vertexPosTextureSize = 2048;
const uint doubleVertexPosTextureSize = vertexPosTextureSize * vertexPosTextureSize;

void main()
{
    ivec3 vertexPosition = ivec3(gl_VertexID % vertexPosTextureSize, (gl_VertexID / vertexPosTextureSize) % vertexPosTextureSize, (gl_VertexID / doubleVertexPosTextureSize));
    vec4 vertexData = imageLoad(vertexPosTexture, vertexPosition);

    vs_out.rawPosition = vertexData.xyz;
    vs_out.intensity = vertexData.w;

	gl_Position = projectionMatrix * viewMatrix * vec4(vertexData.xyz, 1.0);
}