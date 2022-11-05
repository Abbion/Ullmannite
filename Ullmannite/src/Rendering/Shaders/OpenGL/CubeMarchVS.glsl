#version 460 core

layout(rgba32f, binding = 0) readonly uniform image3D vertexPosTexture;

uniform uvec3 vertexPosTextureSize;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 colorT;

void main()
{
    ivec3 vertexPosition = ivec3(gl_VertexID % 2048, (gl_VertexID / 2048) % 2048, (gl_VertexID / (2048 * 2048)));
    vec4 vertex = imageLoad(vertexPosTexture, vertexPosition);

    colorT = vec3(vertex.xyz);
	gl_Position = projectionMatrix * viewMatrix * vertex;
}