#version 400

layout (vertices = 4) out;

uniform vec3 gEyeWorldPosition;

//Inputs
in vec4 Position_CS_in[];
in vec2 Texture_CS_in[];
//in vec3 Normal_CS_in[];

//Outputs
out vec4 Position_ES_in[];
out vec2 Texture_ES_in[];
//out vec3 Normal_ES_in[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    Texture_ES_in[gl_InvocationID] = Texture_CS_in[gl_InvocationID];
    Position_ES_in[gl_InvocationID] = Position_CS_in[gl_InvocationID];

    if(gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 16;
        gl_TessLevelOuter[1] = 16;
        gl_TessLevelOuter[2] = 16;
        gl_TessLevelOuter[3] = 16;

        gl_TessLevelInner[0] = 16;
        gl_TessLevelInner[1] = 16;
    }
}
