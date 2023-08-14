#version 400 core

layout(quads, fractional_odd_spacing, ccw) in; //maybe quads

uniform sampler2D heightMap;
uniform mat4 M;
uniform mat4 PV;
/* uniform mat4 view; */
/* uniform mat4 projection; */

in vec2 Texture_ES_in[];

//out vec4 Position_FS_in;
out vec2 Texture_FS_in;
out float Height; 

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

    vec2 t00 = Texture_ES_in[0];
    vec2 t01 = Texture_ES_in[1];
    vec2 t10 = Texture_ES_in[2];
    vec2 t11 = Texture_ES_in[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 texCoord = (t1 - t0) * v + t0;

    Height = texture(heightMap, texCoord).y * 64.0f - 16.0f;
    /* Texture_FS_in = c0 * (1-u) * (1-v) + c1 * u * (1-v) + c2 * u * v + c3 * (1-u) * v; */

        // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // compute patch surface normal
    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec4 normal = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0) );

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // displace point along normal
    p += normal * Height;

    // ----------------------------------------------------------------------
    // output patch point position in clip space
    gl_Position = PV * p;

    /* vec4 p0 = gl_in[0].gl_Position; */
    /* vec4 p1 = gl_in[1].gl_Position; */
    /* vec4 p2 = gl_in[2].gl_Position; */
    /* vec4 p3 = gl_in[3].gl_Position; */

    /* gl_Position = p0 * (1-u) * (1-v) + p1 * u * (1-v) + p2 * u * v + p3 * (1-u) * v; */
    /* p = p0 * (1-u) * (1-v) + p1 * u * (1-v) + p2 * u * v + p3 * (1-u) * v; */
    /* gl_Position = PV * M * p; */


}
