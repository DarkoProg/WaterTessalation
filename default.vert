#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec2 aTexture;
//Normals
 layout (location = 2) in vec3 aNormal;

// Outputs the color for the Fragment Shader
 out vec3 color;


// Inputs the matrices needed for 3D viewing with perspective
uniform mat4 PV;
 uniform mat4 M;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); 
	// Assigns the colors from the Vertex Data to "color"
	// color = aNormal;
}
