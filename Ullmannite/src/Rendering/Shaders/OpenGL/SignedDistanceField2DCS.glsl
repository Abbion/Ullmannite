#version 460

layout (local_size_x = 16, local_size_y = 16) in;

layout(r16ui, binding = 0) uniform uimage2D inputImage;

uniform uint beta;
uniform ivec2 offset;
uniform uint textureSize;

void main()
{
    //https://prideout.net/blog/distance_fields/#min-erosion
    const ivec2 samplePosition = ivec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);
    ivec2 samplePositionOffset_1 = samplePosition + offset;
    ivec2 samplePositionOffset_2 = samplePosition - offset;

    if (samplePositionOffset_1.x >= textureSize || samplePositionOffset_1.y >= textureSize)
        samplePositionOffset_1 = samplePosition;
    if (samplePositionOffset_2.x < 0 || samplePositionOffset_2.y < 0)
        samplePositionOffset_2 = samplePosition;

    const uint currentSample = imageLoad(inputImage, samplePosition).x;
    const uint offsetSample_1 = imageLoad(inputImage, samplePositionOffset_1).x + beta;
    const uint offsetSample_2 = imageLoad(inputImage, samplePositionOffset_2).x + beta;

    const uint minValue = min(min(currentSample, offsetSample_1), offsetSample_2);

    if(minValue != currentSample)
    {
        imageStore(inputImage, samplePosition, uvec4(minValue));   
    }
}