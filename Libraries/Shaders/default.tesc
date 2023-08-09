#version 400 core

layout (vertices = 4) out;

in vec2 uvs[];
out vec2 uvsCoords[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    uvsCoords[gl_InvocationID] = uvs[gl_InvocationID];
}
