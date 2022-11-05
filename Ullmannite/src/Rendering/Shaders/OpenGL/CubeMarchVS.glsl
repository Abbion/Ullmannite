#version 460 core

layout(rgba32f, binding = 0) readonly uniform image3D vertexPosTexture;

uniform uvec3 vertexPosTextureSize;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * imageLoad(vertexPosTexture, ivec3(gl_VertexID, 0, 0));
}