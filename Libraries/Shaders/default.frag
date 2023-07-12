#version 400 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
//in vec3 color;  
//in vec2 Texture_CS_in;
in float Height;

uniform sampler2D gSampler;

void main()
{
    float heightColor = 125;//(Height + 16) / 32.0f;
	FragColor = vec4(heightColor, heightColor, heightColor, 1.0f); 
    //FragColor = texture(gSampler, Texture_CS_in);
}
