#version 400 core

// Positions/Coordinates
layout (location = 0) in vec3 Position_VS_in;
// Colors
layout (location = 1) in vec2 Texture_VS_in;
//Normals
//layout (location = 2) in vec3 Normal_VS_in;

// Outputs the color for the Fragment Shader
//out vec3 color;
//out vec2 texCord;

//out vec4 Position_CS_in;
//out vec3 Position_CS_in; use when you include tessalation shaders
out vec2 Texture_CS_in;

// Inputs the matrices needed for 3D viewing with perspective
/* uniform mat4 PV; */
/* uniform mat4 M; */

void main()
{
	// Outputs the positions/coordinates of all vertices
	//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); 
//	gl_Position = vec4(Position_VS_in.x, Position_VS_in.y, Position_VS_in.z, 1.0); 
	//Position_CS_in = vec4(Position_VS_in.x, Position_VS_in.y, Position_VS_in.z, 1.0); 
    //Position_CS_in = vec4(Position_VS_in, 1.0); //might change with PMV
    Texture_CS_in = Texture_VS_in;
    gl_Position = vec4(Position_VS_in, 1.0f);
	// Assigns the colors from the Vertex Data to "color"
	// color = aNormal;
}
