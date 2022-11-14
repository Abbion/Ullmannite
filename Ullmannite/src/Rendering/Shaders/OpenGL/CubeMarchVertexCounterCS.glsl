#version 460 core

layout (local_size_x = 16, local_size_y = 8, local_size_z = 8) in;

layout(r16ui, binding = 0) readonly uniform uimage3D inputImage;
layout(r8ui, binding = 1) readonly uniform uimage1D vertexCountTable;
layout(std430, set = 0, binding = 2) writeonly buffer vertexCountStorageBuffer
{
    uint vertexCountStorageData[];
};

struct CubeMarchSettings
{
    uvec3 size;
    uint minSampleVal;
    uint maxSampleVal;
    float maxDataValue;
};

uniform CubeMarchSettings CMsettings;

shared uint localVertexCounter;

void main()
{
    if(gl_GlobalInvocationID.z > CMsettings.size.z || gl_GlobalInvocationID.y > CMsettings.size.y || gl_GlobalInvocationID.x > CMsettings.size.x)
    {
        vertexCountStorageData[gl_WorkGroupID.x + (gl_WorkGroupID.y * gl_NumWorkGroups.x) + (gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y)] = 0;
        return;
    }

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0 && gl_LocalInvocationID.z == 0)
        localVertexCounter = 0;

    memoryBarrierShared();
    barrier();

    const uvec3 cubeCornderSampler[8] = {   uvec3(0, 0, 0), uvec3(1, 0, 0),
                                            uvec3(1, 1, 0), uvec3(0, 1, 0),
                                            uvec3(0, 0, 1), uvec3(1, 0, 1),
                                            uvec3(1, 1, 1), uvec3(0, 1, 1) };
    
    uint activeEdgeCounter = 0;

    for(uint itr = 0; itr < 8; ++itr)
    {
        uint cornderValue = imageLoad(inputImage, ivec3(gl_GlobalInvocationID + ivec3(-1, -1, -1)  + cubeCornderSampler[itr])).x;

        if(cornderValue >= CMsettings.minSampleVal && cornderValue <= CMsettings.maxSampleVal)
            activeEdgeCounter |= 1 << itr;
    }

    uint vertexCountInCube = imageLoad(vertexCountTable, int(activeEdgeCounter)).x;
    atomicAdd(localVertexCounter, vertexCountInCube);

    memoryBarrierShared();
    barrier();

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0 && gl_LocalInvocationID.z == 0)
        vertexCountStorageData[gl_WorkGroupID.x + (gl_WorkGroupID.y * gl_NumWorkGroups.x) + (gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y)] = localVertexCounter;
}