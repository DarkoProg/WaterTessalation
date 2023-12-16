#version 400 core

layout (vertices = 4) out;

uniform vec3 gEyeWorldPosition;

in vec2 Texture_CS_in[];

out vec2 Texture_ES_in[];

void main()
{
    int tessNum = 128;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    Texture_ES_in[gl_InvocationID] = Texture_CS_in[gl_InvocationID];
    
    if(gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = tessNum;
        gl_TessLevelOuter[1] = tessNum;
        gl_TessLevelOuter[2] = tessNum;
        gl_TessLevelOuter[3] = tessNum;

        gl_TessLevelInner[0] = tessNum;
        gl_TessLevelInner[1] = tessNum;
    }
}
