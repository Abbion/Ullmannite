#version 460

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout(r32ui, binding = 0) uniform uimage2D inputImage;

uniform uint threshold;
uniform uint newValue;

void main()
{
    //https://prideout.net/blog/distance_fields/#min-erosion
    const ivec2 samplePosition = ivec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);
    const uint currentSample = imageLoad(inputImage, samplePosition).x;

    if(currentSample > threshold)
    {
        imageStore(inputImage, samplePosition, uvec4(newValue));
    }
}