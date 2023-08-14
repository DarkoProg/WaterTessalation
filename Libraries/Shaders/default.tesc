#version 400 core

layout (vertices = 4) out;

in vec2 uvs[];
out vec2 uvsCoords[];

void main()
{
    int outer = 3;
    int inner = 3;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    uvsCoords[gl_InvocationID] = uvs[gl_InvocationID];

    gl_TessLevelOuter[0] = outer;
    gl_TessLevelOuter[1] = outer;
    gl_TessLevelOuter[2] = outer;
    gl_TessLevelOuter[3] = outer;

    gl_TessLevelInner[0] = inner;
    gl_TessLevelInner[1] = inner;
}
