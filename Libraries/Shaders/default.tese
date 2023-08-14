#version 400 core

layout (quads, equal_spacing, ccw) in;

in vec2 uvsCoord[];
out vec2 uvs;
out

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

    gl_Position = p0 * (1 - u) * (1 - v) + p1 * u * (1 - v) + p2 * u * v + p3 * (1 - u) * v;

    vec3 c0 = uvsCoord[0];
    vec3 c1 = uvsCoord[1];
    vec3 c2 = uvsCoord[2];
    vec3 c3 = uvsCoord[3];

    uvs = c0 * (1 - u) * (1 - v) + c1 * u * (1 - v) + c2 * u * v + c3 * (1 - u) * v;
}
