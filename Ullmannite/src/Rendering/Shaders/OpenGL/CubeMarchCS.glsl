#version 460 core

layout (local_size_x = 16, local_size_y = 8, local_size_z = 8) in;

layout(r16ui, binding = 0) readonly uniform uimage3D inputImage;
layout(r8i, binding = 1) readonly uniform iimage2D triangulationTable;
layout(rgba32f, binding = 2) writeonly uniform image3D vertexPosTexture;
layout(binding = 3) uniform atomic_uint vertexPosTextureItr;

struct CubeMarchSettings
{
    uvec3 size;
    uint minSampleVal;
    uint maxSampleVal;
    float maxDataValue;
};

uniform CubeMarchSettings CMsettings;
//If cutting value is negative that means the plane is inverted
uniform ivec3 cuttingPlanes;

const uint vertexPosTextureSize = 2048;
const uint doubleVertexPosTextureSize = vertexPosTextureSize * vertexPosTextureSize;


const uvec3 cubeCornderSampler[8] = {   uvec3(0, 0, 0), uvec3(1, 0, 0),
                                        uvec3(1, 1, 0), uvec3(0, 1, 0),
                                        uvec3(0, 0, 1), uvec3(1, 0, 1),
                                        uvec3(1, 1, 1), uvec3(0, 1, 1) };

const vec3 edgePosFromNumber[12] = {    vec3(0, 0.5, -0.5), vec3(0.5, 0, -0.5), vec3(0, -0.5, -0.5), vec3(-0.5, 0, -0.5),
                                        vec3(0, 0.5, 0.5),  vec3(0.5, 0, 0.5),  vec3(0, -0.5, 0.5),  vec3(-0.5, 0, 0.5),
                                        vec3(-0.5, 0.5, 0), vec3(0.5, 0.5, 0),  vec3(0.5, -0.5, 0),  vec3(-0.5, -0.5, 0) };

                                          
const uint vertexNumFromEdge[24] = { 0, 1, /*0*/ 2, 1, /*1*/ 3, 2, /*2*/ 3, 0, /*3*/ 4, 5, /*4*/  6, 5, /*5*/
                                     7, 6, /*6*/ 7, 4, /*7*/ 0, 4, /*8*/ 1, 5, /*9*/ 2, 6, /*10*/ 3, 7  /*11*/ };

vec3 interpolateEdge(float c1, float c2, int edgeNum)
{
    float minCorner = min(c1, c2);
    float maxCorner = max(c1, c2);
    
    //Naming is flipped because when we check if max corrner is between sample points than we interpolate minimal values.
    //And when min is between sample points than we interpolate maximal values.
    bool minInterpolation = (CMsettings.minSampleVal <=  maxCorner && CMsettings.maxSampleVal >= maxCorner);
    bool maxInterpolation = (CMsettings.minSampleVal <= minCorner && CMsettings.maxSampleVal >= minCorner);  

    if(minInterpolation || maxInterpolation)
    {
        float CornderDiff = c1 - c2;
        float samplingDiff = 0;

        if(minInterpolation)
            samplingDiff = minCorner - CMsettings.minSampleVal;
        else
            samplingDiff = maxCorner - CMsettings.maxSampleVal;

        float interpolatedPoint = samplingDiff / CornderDiff;

        if(interpolatedPoint < 0)
            interpolatedPoint = 1 + interpolatedPoint;
        
        interpolatedPoint -= 0.5;

        if(edgeNum == 0 || edgeNum == 2 || edgeNum == 4 || edgeNum == 6)
            return vec3(1, 0, 0) * interpolatedPoint;
        else if(edgeNum == 1 || edgeNum == 3 || edgeNum == 5 || edgeNum == 7)
            return vec3(0, 1, 0) * interpolatedPoint;
        else
            return vec3(0, 0, 1) * interpolatedPoint;

        //TODO INTERPOLATE EDGE VALUES!!!
    }
}

void main() 
{
    if(gl_GlobalInvocationID.z > CMsettings.size.z || gl_GlobalInvocationID.y > CMsettings.size.y || gl_GlobalInvocationID.x > CMsettings.size.x)
        return;

    if(((cuttingPlanes.x >= 0 && gl_GlobalInvocationID.x > cuttingPlanes.x) || (cuttingPlanes.x < 0 && gl_GlobalInvocationID.x < -cuttingPlanes.x)) ||
        ((cuttingPlanes.y >= 0 && gl_GlobalInvocationID.y > cuttingPlanes.y) || (cuttingPlanes.y < 0 && gl_GlobalInvocationID.y < -cuttingPlanes.y)) ||
        ((cuttingPlanes.z >= 0 && gl_GlobalInvocationID.z > cuttingPlanes.z) || (cuttingPlanes.z < 0 && gl_GlobalInvocationID.z < -cuttingPlanes.z)))
        return;

    uint activeEdgeCounter = 0;

    ivec3 globalPositionI = ivec3(gl_GlobalInvocationID  + ivec3(-1, -1, -1));

    for(uint itr = 0; itr < 8; ++itr)
    {
        ivec3 samplePoint = ivec3(globalPositionI + cubeCornderSampler[itr]);
        uint cornderValue = imageLoad(inputImage, samplePoint).x;

        if(cornderValue >= CMsettings.minSampleVal && cornderValue <= CMsettings.maxSampleVal)
            activeEdgeCounter |= 1 << itr;
    }

    vec4 edgePointPositions[16];
    vec3 globalPositionF = vec3(gl_GlobalInvocationID) + vec3(-1, -1, -1);

    vec3 recenter = vec3(CMsettings.size.x - 1, CMsettings.size.y -1, CMsettings.size.z -1);
    if(recenter.x < 2) recenter.x = 2;
    if(recenter.y < 2) recenter.y = 2;
    if(recenter.z < 2) recenter.z = 2;

    uint Xitr;
    float divider = 2.0 * CMsettings.maxDataValue; // Normalized average

    for(Xitr = 0; Xitr < 16; ++Xitr)
    {
        int edgeNum = imageLoad(triangulationTable, ivec2(Xitr, activeEdgeCounter)).x;

        if(edgeNum == -1)
            break;

        ivec3 corner1 = ivec3(cubeCornderSampler[vertexNumFromEdge[2 * edgeNum]]) + globalPositionI;
        ivec3 corner2 = ivec3(cubeCornderSampler[vertexNumFromEdge[(2 * edgeNum) + 1]]) + globalPositionI;

        float corner1Value = imageLoad(inputImage, corner1).x;
        float corner2Value = imageLoad(inputImage, corner2).x;

        float edgeValue = (corner1Value + corner2Value) / divider;

        vec3 interpolatedVec = interpolateEdge(corner1Value, corner2Value, edgeNum);

        edgePointPositions[Xitr] = vec4(((vec3(globalPositionF.x + 0.5, -globalPositionF.y - 0.5, globalPositionF.z + 0.5) + edgePosFromNumber[edgeNum] + interpolatedVec) / recenter) - vec3(0.5, -0.5, 0.5), edgeValue);
    }

    if(Xitr == 0)
        return;

    uint itrStartPosition = atomicCounterAdd(vertexPosTextureItr, Xitr);

    for(uint posItr = 0; posItr < Xitr; ++posItr)
    {
        ivec3 savePositionInTextureCorrds =  ivec3((itrStartPosition + posItr) % vertexPosTextureSize, ((itrStartPosition + posItr) / vertexPosTextureSize) % vertexPosTextureSize, (itrStartPosition + posItr) / doubleVertexPosTextureSize);
        
        imageStore(vertexPosTexture, savePositionInTextureCorrds, edgePointPositions[posItr]);
    }
}