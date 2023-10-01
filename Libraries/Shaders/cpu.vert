#version 400 core

layout (location = 0) in vec3 Position_VS_in;

out vec3 Position_FS_in ;

uniform mat4 PV;

void main()
{
    gl_Position = PV * vec4(Position_VS_in, 1.0f);
    Position_FS_in = Position_VS_in;
}
