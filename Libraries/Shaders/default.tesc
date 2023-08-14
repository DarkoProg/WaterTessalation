#version 400 core

layout (vertices = 4) out;

uniform vec3 gEyeWorldPosition;

//Inputs
//in vec4 Position_CS_in[];
in vec2 Texture_CS_in[];
//in vec3 Normal_CS_in[];

//Outputs
//out vec4 Position_ES_in[];
out vec2 Texture_ES_in[];
//out vec3 Normal_ES_in[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    Texture_ES_in[gl_InvocationID] = Texture_CS_in[gl_InvocationID];
    //Position_ES_in[gl_InvocationID] = Position_CS_in[gl_InvocationID];

    gl_TessLevelOuter[0] = 1;
    gl_TessLevelOuter[1] = 1;
    gl_TessLevelOuter[2] = 1;
    gl_TessLevelOuter[3] = 1;

    gl_TessLevelInner[0] = 1;
    gl_TessLevelInner[1] = 1;
}
