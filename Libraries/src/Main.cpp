#include "../include/glad.h"
#include "glm/gtx/string_cast.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/Camera.h"
#include "../include/EBO.h"
#include "../include/VAO.h"
#include "../include/VBO.h"
#include "../include/shaderClass.h"
#include "../include/Wave.h"
 
const unsigned int width = 1920;
const unsigned int height = 1080;


/* GLfloat vertices[] = */
/* { */
/* 	-0.5f, -0.5f , 0.0f, 0.0f, 0.0f, // Lower left corner */
/* 	 0.5f, -0.5f , 0.0f, 1.0f, 0.0f, // Lower right corner */
/* 	 0.5f,  0.5f , 0.0f, 1.0f, 1.0f, // Upper right */
/* 	-0.5f,  0.5f , 0.0f, 0.0f, 1.0f  // Upper left */
/* }; */

GLfloat vertices[] =
{
	-0.5f, 0.0f , -0.5f, 0.0f, 0.0f, // Lower left corner
	 0.5f, 0.0f , -0.5f, 1.0f, 0.0f, // Lower right corner
	 0.5f, 0.0f ,  0.5f, 1.0f, 1.0f, // Upper right
	-0.5f, 0.0f ,  0.5f, 0.0f, 1.0f  // Upper left
};

GLFWwindow* window; 

GLFWwindow* GLInit()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* windowi = glfwCreateWindow(800, 800, "Diplomska", NULL, NULL);
	// Error check if the window fails to create
	if (windowi == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(windowi);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);

    GLint MaxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
    printf("Max supported patch vertices %d\n", MaxPatchVertices);

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    return windowi;
}

unsigned int TextureInit()
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

void TextureSetup(int widthImg, int heightImg, Shader& shaderProgram)
{
    Wave wave;
    int data[widthImg] [heightImg];
    wave.test(*data);
    if (*data)
    {
        /* std::cout << "zakaj"; */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    GLuint textureToUni = glGetUniformLocation(shaderProgram.ID, "heightMap");
    glUniform1i(textureToUni, 0);
}

int main()
{
    unsigned numberOfPatches = 1;

    Wave wave;

    window = GLInit();
	Shader shaderProgram("Libraries/Shaders/default.vert", "Libraries/Shaders/default.frag", "Libraries/Shaders/default.tesc", "Libraries/Shaders/default.tese");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
    VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    VAO1.LinkAttribute(VBO1, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();

    int widthImg = 1000;
    int heightImg= 1000;
    TextureSetup(widthImg, heightImg, shaderProgram);
    shaderProgram.Activate();

    float scale = 0.5f;
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));



	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        camera.Inputs(window, scale);

        camera.Matrix(45.5f, 0.1f, 100.0f, shaderProgram, "PV");
		camera.Inputs(window, scale);

        //just wirefram testing
        /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
		VAO1.Bind();
		glDrawArrays(GL_PATCHES, 0, 4); //maybe wrong

		glfwSwapBuffers(window);
		glfwPollEvents();

        /* std::cout << "x: " << camera.Position.x << " y: " << camera.Position.y << " z: " << camera.Position.z << std::endl; */
	}

	VAO1.Delete();
	VBO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

