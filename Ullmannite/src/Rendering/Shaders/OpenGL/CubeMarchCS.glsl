#version 460

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(r16ui, binding = 0) uniform uimage3D inputImage;
layout(r8i, binding = 1) uniform iimage2D triangulationTable;

struct CubeVertices
{
    int size;
    vec3 vectices[5];   //Max amount of verices in cube
};

precision highp int;

void main() 
{
    ivec3 inputImageSize = imageSize(inputImage);
}