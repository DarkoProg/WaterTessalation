#version 400 core

layout(quads, fractional_odd_spacing, ccw) in; //maybe quads

uniform sampler2D heightMap;
uniform mat4 M;
uniform mat4 PV;

in vec2 Texture_ES_in[];

out float Height; 

float BilinearInterpolation(float u, float v)
{
    vec2 t00 = Texture_ES_in[0];
    vec2 t01 = Texture_ES_in[1];
    vec2 t11 = Texture_ES_in[2];
    vec2 t10 = Texture_ES_in[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 textureCordinate = (t1 - t0) * v + t0;
    /* vec2 textureCordinate = v * ((1-u) * t00 + u * t01) - (1-v) * ((1-u) * t10 + u * t11); */
    /* vec2 textureCordinate = u * ((1-v) * t00 + v * t01) - (1-u) * ((1-v) * t10 + v * t11); */
    return texture(heightMap, textureCordinate).x;
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    Height = BilinearInterpolation(u,v) * 5 + 2;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p11 = gl_in[2].gl_Position;
    vec4 p10 = gl_in[3].gl_Position;

    // compute patch surfaceunormal
    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec4 normal = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0) );

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // displace point along normal
    p += normal * Height;

    /* // ---------------------------------------------------------------------- */
    /* // output patch point position in clip space */
    gl_Position = PV * p;

    /* vec4 p0 = gl_in[0].gl_Position; */
    /* vec4 p1 = gl_in[1].gl_Position; */
    /* vec4 p2 = gl_in[2].gl_Position; */
    /* vec4 p3 = gl_in[3].gl_Position; */

    /* gl_Position = p0 * (1-u) * (1-v) + p1 * u * (1-v) + p2 * u * v + p3 * (1-u) * v; */


}
