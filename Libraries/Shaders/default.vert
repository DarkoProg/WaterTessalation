#version 400 core

layout (location = 0) in vec3 Position_VS_in;
layout (location = 1) in vec2 Texture_VS_in;

out vec2 Texture_CS_in;


void main()
{
    gl_Position = vec4(Position_VS_in, 1.0f);
    Texture_CS_in = Texture_VS_in;
}
