#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;  

void main()
{
	FragColor = vec4(0.8f,0.3f, 0.02f, 1.0f); 
}
