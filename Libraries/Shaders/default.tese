#version 400 core

layout(quads, equal_spacing, ccw) in; //maybe quads

uniform sampler2D heightMap;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


//in vec4 Position_ES_in[];
in vec2 Texture_ES_in[];
//in vec3 Normal_ES_in;

//out vec4 Position_FS_in;
out vec2 Texture_FS_in;
/* out float Height; */
//in vec3 Normal_FS_in;

vec2 InterpolateVec2(float uCordinate, float vCordinate, vec2 texCord00, vec2 texCord01, vec2 texCord10, vec2 texCord11)
{
    vec2 texCord0 = (texCord01 - texCord00) * uCordinate + texCord00;
    vec2 texCord1 = (texCord11 - texCord10) * uCordinate + texCord10;
    vec2 interpolatedCord = (texCord1 - texCord0) * vCordinate + texCord0;
    return interpolatedCord;
}

vec4 InterpolateVec4(float uCordinate, float vCordinate, vec4 position00, vec4 position01, vec4 position10, vec4 position11)
{
    vec4 position0 = (position01 - position00) * uCordinate + position00;
    vec4 position1 = (position11 - position10) * uCordinate + position10;
    vec4 interpolatedCord = (position1 - position0) * vCordinate + position0;
    return interpolatedCord;
}

vec4 ComputeNormal(vec4 position00, vec4 position01, vec4 position10)
{
    vec4 uPatchVec   = position01 - position00;
    vec4 vPatchVec   = position10 - position00;
    vec4 patchNormal = normalize( vec4(cross(vPatchVec.xyz, uPatchVec.xyz), 0) );
    return patchNormal;
}

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}
void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

    gl_Position = p0 * (1-u) * (1-v) + p1 * u * (1-v) + p2 * u * v + p3 * (1-u) * v;

    vec2 c0 = Texture_ES_in[0];
    vec2 c1 = Texture_ES_in[1];
    vec2 c2 = Texture_ES_in[2];
    vec2 c3 = Texture_ES_in[3];

    Texture_FS_in = c0 * (1-u) * (1-v) + c1 * u * (1-v) + c2 * u * v + c3 * (1-u) * v;
}
