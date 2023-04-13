#version 400

layout (verticies = 3) out;

uniform vec3 gEyeWorldPosition;

//Inputs
in vec3 Position_CS_in[];
in vec2 Texture_CS_in[];
//in vec3 Normal_CS_in[];

//Outputs
out vec3 Position_ES_in[];
out vec2 Texture_ES_in[];
//out vec3 Normal_ES_in[];

float getTessLevel(float distance0, float distance1)
{
    float AverageDistance = (distance0 + distance1) / 2;

    if(AverageDistance <= 2.0)
    {
        return 10.0;
    }
    else if(AverageDistance <= 5.0)
    {
        return 7.0;
    }
    ekse
    {
        return 3.0;
    }
}

void main()
{
    Texture_ES_in[gl_InvocationID] = Texture_CS_in[gl_InvocationID];
    //Normal_ES_in[gl_InvocationID] = Normal_CS_in[gl_InvocationID];
    Position_ES_in[gl_InvocationID] = Position_CS_in[gl_InvocationID];

    //distance to control poitns
    float eyeToVertexDistance0 = distance(gEyeWorldPosition, Position_ES_in[0]);
    float eyeToVertexDistance1 = distance(gEyeWorldPosition, Position_ES_in[1]);
    float eyeToVertexDistance2 = distance(gEyeWorldPosition, Position_ES_in[2]);
    float eyeToVertexDistance3 = distance(gEyeWorldPosition, Position_ES_in[3]);

    //tessalation levels
    gl_TessLevelOuter[0] = getTessLevel(eyeToVertexDistance0, eyeToVertexDistance1);
    gl_TessLevelOuter[1] = getTessLevel(eyeToVertexDistance1, eyeToVertexDistance3);
    gl_TessLevelOuter[2] = getTessLevel(eyeToVertexDistance3, eyeToVertexDistance2);
    gl_TessLevelOuter[3] = getTessLevel(eyeToVertexDistance2, eyeToVertexDistance0);
    //this is for triangle, do it for square
}
