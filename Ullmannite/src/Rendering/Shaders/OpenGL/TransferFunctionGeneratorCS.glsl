#version 460 core

layout (local_size_x = 16, local_size_y = 1, local_size_z = 1) in;

struct TransferPoint{
    vec3 color;
    float position;
};

layout(rgba32f, binding = 0) writeonly uniform image1D transferFunctionTexture;
layout(std430, binding = 1) readonly buffer transferPointBuffer
{
    TransferPoint transferPointData[];
};

uniform uint colorPointsCount;
uniform float textureWidth;

void main()
{
    if(gl_GlobalInvocationID.x > colorPointsCount - 2)
        return;

    TransferPoint transferPoint = transferPointData[gl_GlobalInvocationID.x];
    TransferPoint nextTransferPoint = transferPointData[gl_GlobalInvocationID.x + 1];

    uint transferPointPosition = uint(transferPoint.position * textureWidth);
    uint nextTransferPointPosition = uint(nextTransferPoint.position * textureWidth);

    float diff = float(nextTransferPointPosition - transferPointPosition);
    int mixItr = 0;

    for(uint itr = transferPointPosition; itr < nextTransferPointPosition; ++itr, ++mixItr)
    {
        float mixRatio = float(mixItr) / diff;
        imageStore(transferFunctionTexture, int(itr), vec4(mix(transferPoint.color, nextTransferPoint.color, mixRatio), 1.0));
    }

    if(gl_GlobalInvocationID.x == colorPointsCount - 2 && nextTransferPointPosition < 512)
    {
        for(uint itr = nextTransferPointPosition; itr < 512; ++itr)
            imageStore(transferFunctionTexture, int(itr), vec4(nextTransferPoint.color, 1.0));
    }
    else if(gl_GlobalInvocationID.x == 0 && transferPointPosition > 0)
    {
        for(uint itr = 0; itr < transferPointPosition; ++itr)
            imageStore(transferFunctionTexture, int(itr), vec4(transferPoint.color, 1.0));
    }
}