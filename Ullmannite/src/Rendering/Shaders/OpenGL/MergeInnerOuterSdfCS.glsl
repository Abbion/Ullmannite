#version 460

layout (local_size_x = 16, local_size_y = 16) in;

layout(r16ui, binding = 0) readonly uniform uimage2D outerSdf;
layout(r16ui, binding = 1) readonly uniform uimage2D innerSdf;
layout(r32f, binding = 2) writeonly uniform image2D outputImage;

void main()
{
    const ivec2 samplePosition = ivec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);
    const uint outerSdfSample = imageLoad(outerSdf, samplePosition).x;
    const uint innerSdfSample = imageLoad(innerSdf, samplePosition).x;
    
    imageStore(outputImage, samplePosition, vec4(sqrt(outerSdfSample + innerSdfSample)));
}