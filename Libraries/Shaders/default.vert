#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCords;

out vec2 uvs;


void main()
{
    gl_Position = vec4(pos.xyz, 1.0);
    uvs = texCords;
}
