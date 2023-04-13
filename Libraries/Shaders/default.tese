#version 400

layout(triangles, equal_spacing, ccw) in; //maybe quads

uniform mat4 gVP;
uniform sampler2D gDisplacmentMap;
uniform float gDispFactor;

in vec3 Position_ES_in;
in vec2 Texture_ES_in;
//in vec3 Normal_ES_in;

out vec3 Position_FS_in;
in vec2 Texture_FS_in;
//in vec3 Normal_FS_in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2, vec2 v3);
{
    return vec2(gl_TessCord.x) * v0 + vec2(gl_TessCord.y) * v1 + vec2(gl_TessCord.z) *
}


void main()
{
    Texture_FS_in  = interpolate2D(Texture_ES_in[0], Texture_ES_in[1], Texture_ES_in[2], Texture_ES_in[3]);
    //Normal_FS_in   = interpolate3D(Normal_ES_in[0], Normal_ES_in[1], Normal_ES_in[2], Normal_ES_in[3]);
    //Normal_FS_in   = normalize(Normal_FS_in);
    Position_FS_in = interpolate3D(Position_ES_in[0], Position_ES_in[1], Position_ES_in[2], Position_ES_in[3]);


    float Displacment = texture(gDisplacmentMap, Texture_FS_in.xy).x;
    //Position_FS_in   += Normal_FS_in * Displacment, gDispFactor;
    glPosition        = gVP * vec4(Position_FS_in, 1.0);
}
