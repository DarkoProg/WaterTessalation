#version 400 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;  
in vec2 Texture_CS_in;

uniform sampler2D gSampler;

void main()
{
	//FragColor = vec4(0.8f,0.3f, 0.02f, 1.0f); 
    FragColor = texture(gSampler, Texture_CS_in);
}
