#version 460

layout (local_size_x = 16, local_size_y = 8, local_size_z = 8) in;

layout(r16ui, binding = 0) readonly uniform uimage3D inputImage;
layout(r8i, binding = 1) readonly uniform iimage2D triangulationTable;
layout(rgba32f, binding = 2) writeonly uniform image3D vertexPosTexture;
layout(binding = 3) uniform atomic_uint vertexPosTextureItr;

struct CubeMarchSettings
{
    uvec3 size;
    uint sampleJump;
    uint minSampleVal;
    uint maxSampleVal;
};

uniform CubeMarchSettings CMsettings;

const uint vertexPosTextureSize = 2048;
const uint doubleVertexPosTextureSize = vertexPosTextureSize * vertexPosTextureSize;

void main() 
{
    if(gl_GlobalInvocationID.z > CMsettings.size.z || gl_GlobalInvocationID.y > CMsettings.size.y || gl_GlobalInvocationID.x > CMsettings.size.x)
        return;

    const uvec3 cubeCornderSampler[8] = {   uvec3(0, 0, 0), uvec3(1, 0, 0),
                                            uvec3(1, 1, 0), uvec3(0, 1, 0),
                                            uvec3(0, 0, 1), uvec3(1, 0, 1),
                                            uvec3(1, 1, 1), uvec3(0, 1, 1) };
    
    uint activeEdgeCounter = 0;

    for(uint itr = 0; itr < 8; ++itr)
    {
        uint cornderValue = imageLoad(inputImage, ivec3(gl_GlobalInvocationID  + ivec3(-1, -1, -1) + cubeCornderSampler[itr])).x;

        if(cornderValue >= CMsettings.minSampleVal && cornderValue <= CMsettings.maxSampleVal)
            activeEdgeCounter |= 1 << itr;
    }

    const vec3 edgePosFromNumber[12] = {    vec3(0, 0.5, -0.5), vec3(0.5, 0, -0.5), vec3(0, -0.5, -0.5), vec3(-0.5, 0, -0.5),
                                            vec3(0, 0.5, 0.5),  vec3(0.5, 0, 0.5),  vec3(0, -0.5, 0.5),  vec3(-0.5, 0, 0.5),
                                            vec3(-0.5, 0.5, 0), vec3(0.5, 0.5, 0),  vec3(0.5, -0.5, 0),  vec3(-0.5, -0.5, 0) };

    vec3 edgePointPositions[16];
    vec3 globalPositionF = vec3(gl_GlobalInvocationID) + vec3(-1, -1, -1);

    uint Xitr;
    vec3 recenter = vec3(CMsettings.size.x - 1, CMsettings.size.y -1,  CMsettings.size.y -1);
    if(recenter.x < 2)
        recenter.x = 2;
    if(recenter.y < 2)
        recenter.y = 2;
    if(recenter.z< 2)
        recenter.z = 2;

    for(Xitr = 0; Xitr < 16; ++Xitr)
    {
        int edgeNum = imageLoad(triangulationTable, ivec2(Xitr, activeEdgeCounter)).x;

        if(edgeNum == -1)
            break;

        edgePointPositions[Xitr] = ((vec3(globalPositionF.x + 0.5, -globalPositionF.y - 0.5, globalPositionF.z + 0.5) + edgePosFromNumber[edgeNum]) / recenter) - vec3(0.5, -0.5, 0.5);
    }

    if(Xitr == 0)
        return;

    uint itrStartPosition = atomicCounterAdd(vertexPosTextureItr, Xitr);

    for(uint posItr = 0; posItr < Xitr; ++posItr)
    {
        ivec3 savePositionInTextureCorrds =  ivec3((itrStartPosition + posItr) % vertexPosTextureSize, ((itrStartPosition + posItr) / vertexPosTextureSize) % vertexPosTextureSize, (itrStartPosition + posItr) / doubleVertexPosTextureSize);
        
        imageStore(vertexPosTexture, savePositionInTextureCorrds, vec4(edgePointPositions[posItr].xyz , 1.0));
    }
}