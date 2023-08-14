#version 400 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
//in vec3 color;  
//in vec2 Texture_CS_in;
in float Height;
/* in vec2 Texture_FS_in; */

/* uniform sampler2D gSampler; */

void main()
{
    /* float heightColor = 125;//(Height + 16) / 32.0f; */
	/* FragColor = vec4(heightColor, heightColor, heightColor, 1.0f); */ 
    float h = (Height + 16)/64.0f;
	FragColor = vec4(h, h, h, 1.0);
    /* FragColor = vec4(Texture_FS_in, 0.02f, 1.0f); */
    //FragColor = texture(gSampler, Texture_CS_in);
}
