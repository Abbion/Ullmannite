#version 460

layout (local_size_x = 16, local_size_y = 16) in;

layout(r16ui, binding = 0) readonly uniform uimage2D inputImage;
layout(r16ui, binding = 1) writeonly uniform uimage2D outputImage;

uniform uint maxValue;

void main()
{
    const ivec2 samplePosition = ivec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);
    const uint currentSample = imageLoad(inputImage, samplePosition).x;
    
    imageStore(outputImage, samplePosition, uvec4(maxValue - currentSample));
}