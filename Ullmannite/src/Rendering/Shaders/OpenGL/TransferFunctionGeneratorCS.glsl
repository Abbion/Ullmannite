#version 460

layout (local_size_x = 16, local_size_y = 1, local_size_z = 1) in;

struct TransferPoint{
    vec3 color;
    uint position;   
};

layout(rgba32f, binding = 0) writeonly uniform image1D transferFunctionTexture;
layout(std430, binding = 1) readonly buffer transferPointBuffer
{
    TransferPoint transferPointData[];
};

uniform uint colorPointsCount;

void main()
{
    if(gl_GlobalInvocationID.x > colorPointsCount - 2)
        return;

    TransferPoint transferPoint = transferPointData[gl_GlobalInvocationID.x];
    TransferPoint nextTransferPoint = transferPointData[gl_GlobalInvocationID.x + 1];

    float diff = float(nextTransferPoint.position - transferPoint.position);
    int mixItr = 0;

    for(uint itr = transferPoint.position; itr < nextTransferPoint.position; ++itr, ++mixItr)
    {
        float mixRatio = float(mixItr) / diff;
        imageStore(transferFunctionTexture, int(itr), vec4(mix(transferPoint.color, nextTransferPoint.color, mixRatio), 1.0));
    }

    if(gl_GlobalInvocationID.x == colorPointsCount - 2 && nextTransferPoint.position < 512)
    {
        for(uint itr = nextTransferPoint.position; itr < 512; ++itr)
            imageStore(transferFunctionTexture, int(itr), vec4(nextTransferPoint.color, 1.0));
    }
    else if(gl_GlobalInvocationID.x == 0 && transferPoint.position > 0)
    {
        for(uint itr = 0; itr < transferPoint.position; ++itr)
            imageStore(transferFunctionTexture, int(itr), vec4(transferPoint.color, 1.0));
    }
}