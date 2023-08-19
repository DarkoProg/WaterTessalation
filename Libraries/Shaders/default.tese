#version 400 core

layout(quads, equal_spacing, ccw) in; //maybe quads

uniform sampler2D heightMap;
uniform mat4 M;
uniform mat4 PV;
/* uniform mat4 view; */
/* uniform mat4 projection; */

in vec2 Texture_ES_in[];
/* in vec3 Position_CS_in[]; */

//out vec4 Position_FS_in;
out vec2 Texture_FS_in;
out float Height; 
/* out sampler2D tex; */

float BilinearInterpolation(float u, float v)
{
    vec2 t00 = Texture_ES_in[0];
    vec2 t01 = Texture_ES_in[1];
    vec2 t11 = Texture_ES_in[3];
    vec2 t10 = Texture_ES_in[2];

    /* vec2 t0 = (t01.x - u) / (t01.x - t00.x) * t00 + (u - t00.x) / (t01.x - t00.x) * t01; */
    /* vec2 t1 = (t11.x - u) / (t11.x - t10.x) * t10 + (u - t10.x) / (t11.x - t10.x) * t11; */

    /* vec2 textureCordinate = (t1.y - v) / (t1.y - t0.y) * t0 + (v - t1.y) / (t1.y - t0.y) * t1; */
    /* vec2 textureCordinate = (1-v) * ((1-u) * t00 + u * t01) + v * ((1-u) * t10 + u * t11); */
    vec2 textureCordinate = v * ((1-u) * t00 + u * t01) - (1-v) * ((1-u) * t10 + u * t11);
    return texture(heightMap, textureCordinate).z;
    /* return textureCordinate.y; */
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    Height = BilinearInterpolation(u,v) * 0.05f;
    /* tex = heightMap; */
    /* Height = gl_TessCoord.y; */
    /* Texture_ES_in = BilinearInterpolation(u,v); */

    /* vec2 t00 = Texture_ES_in[0]; */
    /* vec2 t01 = Texture_ES_in[1]; */
    /* vec2 t10 = Texture_ES_in[2]; */
    /* vec2 t11 = Texture_ES_in[3]; */
    /* /1* vec2 t11 = Texture_ES_in[2]; *1/ */
    /* /1* vec2 t10 = Texture_ES_in[3]; *1/ */

    /* /1* vec2 t0 = (t01 - t00) * u + t00; *1/ */
    /* /1* vec2 t1 = (t11 - t10) * u + t10; *1/ */
    /* vec2 t0 = (t01 - t00) * u + t00; */
    /* vec2 t1 = (t11 - t10) * u + t10; */
    /* vec2 texCoord = (t1 - t0) * v + t0; */
    /* /1* Height = texCoord.x/100; *1/ */
    /* /1* vec2 texCoord = t00 * (1-u) * (1-v) + t01 * u * (1-v) + t11 * u * v + t10 * (1-u) * v; *2/ *1/ */

    /* Height = texture(heightMap, texCoord/100).y * 0.2; */
    /* Texture_FS_in = c0 * (1-u) * (1-v) + c1 * u * (1-v) + c2 * u * v + c3 * (1-u) * v; */

        // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p11 = gl_in[2].gl_Position;
    vec4 p10 = gl_in[3].gl_Position;
    /* vec4 p10 = gl_in[2].gl_Position; */
    /* vec4 p11 = gl_in[3].gl_Position; */

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
    /* p = p0 * (1-u) * (1-v) + p1 * u * (1-v) + p2 * u * v + p3 * (1-u) * v; */
    /* gl_Position = PV * p; */


}
