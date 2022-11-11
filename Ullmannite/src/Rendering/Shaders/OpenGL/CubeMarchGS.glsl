#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
    vec3 rawPosition;
    float intensity;
} gs_in[];

out GS_OUT{
    vec3 normal;
    float intensity;
} gs_out;

void main()
{
    vec3 normalVec = normalize(cross(gs_in[1].rawPosition - gs_in[2].rawPosition, gs_in[0].rawPosition - gs_in[1].rawPosition));

    gl_Position = gl_in[0].gl_Position;
    gs_out.intensity = gs_in[0].intensity;
    gs_out.normal = normalVec;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    gs_out.intensity = gs_in[1].intensity;
    gs_out.normal = normalVec;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    gs_out.intensity = gs_in[2].intensity;
    gs_out.normal = normalVec;
    EmitVertex();

    EndPrimitive();
}