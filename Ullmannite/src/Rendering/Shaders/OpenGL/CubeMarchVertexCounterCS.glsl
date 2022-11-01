#version 460

layout (local_size_x = 16, local_size_y = 8, local_size_z = 8) in;

layout(r16ui, binding = 0) readonly uniform uimage3D inputImage;
layout(r8ui, binding = 1) readonly uniform uimage1D vertexCountTable;
layout(std430, binding = 2) buffer storageTest
{
    uint storageDataTest[];
};

struct CubeMarchSettings
{
    uvec3 size;
    uint sampleJump;
    uint minSampleVal;
    uint maxSampleVal;
};

uniform CubeMarchSettings CMsettings;

shared uint localVertexCounter;

void main()
{

    uint edge = 1 + CMsettings.sampleJump;
    uvec3 globalID = gl_GlobalInvocationID * CMsettings.sampleJump;

    if(globalID.z > (CMsettings.size.z - edge) || globalID.y > (CMsettings.size.y - edge) || globalID.x > (CMsettings.size.x - edge))
        return;

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0 && gl_LocalInvocationID.z == 0)
        localVertexCounter = 0;

    memoryBarrierShared();
    barrier();

    const uvec3 cubeCornderSampler[8] = {   uvec3(0, 0, 0), uvec3(CMsettings.sampleJump, 0, 0),
                                            uvec3(0, CMsettings.sampleJump, 0), uvec3(CMsettings.sampleJump, CMsettings.sampleJump, 0),
                                            uvec3(0, 0, CMsettings.sampleJump), uvec3(CMsettings.sampleJump, 0, CMsettings.sampleJump),
                                            uvec3(0, CMsettings.sampleJump, CMsettings.sampleJump), uvec3(CMsettings.sampleJump, CMsettings.sampleJump, CMsettings.sampleJump) };
    
    uint activeEdgeCounter = 0;

    for(uint itr = 0; itr < 8; ++itr)
    {
        uint cornderValue = imageLoad(inputImage, ivec3(globalID + cubeCornderSampler[itr])).x;

        if(cornderValue >= CMsettings.minSampleVal && cornderValue <= CMsettings.maxSampleVal)
            activeEdgeCounter |= 1 << itr;
    }

    uint vertexCountInCube = imageLoad(vertexCountTable, int(activeEdgeCounter)).x;
    atomicAdd(localVertexCounter, vertexCountInCube);

    memoryBarrierShared();
    barrier();

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0 && gl_LocalInvocationID.z == 0)
        storageDataTest[gl_WorkGroupID.x + (gl_WorkGroupID.y * gl_NumWorkGroups.x) + (gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y)] = localVertexCounter;
}