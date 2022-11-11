#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
    vec3 colorT;
} gs_in[];

out vec3 colorTA;
out vec3 normal;

void main()
{
    vec3 normalVec = normalize(cross( gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz));

    gl_Position = gl_in[0].gl_Position;
    colorTA = gs_in[0].colorT;
    normal = normalVec;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    colorTA = gs_in[1].colorT;
    normal = normalVec;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    colorTA = gs_in[2].colorT;
    normal = normalVec;
    EmitVertex();


    EndPrimitive();
}