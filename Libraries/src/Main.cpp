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
GLFWwindow* window; 

/* GLfloat vertices[] = */
/* { */
/* 	-0.5f, -0.5f , 0.0f, 0.0f, 0.0f, // Lower left corner */
/* 	 0.5f, -0.5f , 0.0f, 1.0f, 0.0f, // Lower right corner */
/* 	 0.5f,  0.5f , 0.0f, 1.0f, 1.0f, // Upper right */
/* 	-0.5f,  0.5f , 0.0f, 0.0f, 1.0f  // Upper left */
/* }; */

/* GLfloat vertices[] = */
/* { */
/* 	-10.0f, 0.0f , -10.0f, 0.0f, 0.0f, // Lower left corner */
/* 	 10.0f, 0.0f , -10.0f, 1.0f, 0.0f, // Lower right corner */
/* 	 10.0f, 0.0f ,  10.0f, 1.0f, 1.0f, // Upper right */
/* 	-10.0f, 0.0f ,  10.0f, 0.0f, 1.0f  // Upper left */
/* }; */

/* GLfloat vertices[] = */
/* { */
/* 	-0.5f, 0.0f , -0.5f, 0.0f, 0.0f, // Lower left corner */
/* 	 0.5f, 0.0f , -0.5f, 1.0f, 0.0f, // Lower right corner */
/* 	 0.5f, 0.0f ,  0.5f, 1.0f, 1.0f, // Upper right */
/* 	-0.5f, 0.0f ,  0.5f, 0.0f, 1.0f  // Upper left */
/* }; */

std::vector<GLfloat> vertices;
std::vector<unsigned int> indices;

void MakePatches(int patchNum, int imgHeight, int imgWidth)
{
   for(int i = 0; i < patchNum; i++)
   {
        for(int j = 0; j < patchNum; j++)
        {

            //x
            //y
            //z
            //u
            //v
            vertices.push_back(-imgWidth/2.0f + imgWidth*j/(float)patchNum);
            vertices.push_back(0.0f);
            vertices.push_back(-imgHeight/2.0f + imgHeight*i/(float)patchNum);
            vertices.push_back(j / (float)patchNum);
            vertices.push_back(i / (float)patchNum);

            vertices.push_back(-imgWidth/2.0f + imgWidth*(j+1)/(float)patchNum);
            vertices.push_back(0.0f);
            vertices.push_back(-imgHeight/2.0f + imgHeight*i/(float)patchNum);
            vertices.push_back((j+1) / (float)patchNum);
            vertices.push_back(i / (float)patchNum);

            vertices.push_back(-imgWidth/2.0f + imgWidth*(j+1)/(float)patchNum);
            vertices.push_back(0.0f);
            vertices.push_back(-imgHeight/2.0f + imgHeight*(i+1)/(float)patchNum);
            vertices.push_back((j+1) / (float)patchNum);
            vertices.push_back((i+1) / (float)patchNum);
                                                         
            vertices.push_back(-imgWidth/2.0f + imgWidth*j/(float)patchNum);
            vertices.push_back(0.0f);
            vertices.push_back(-imgHeight/2.0f + imgHeight*(i+1)/(float)patchNum);
            vertices.push_back(j / (float)patchNum);
            vertices.push_back((i+1) / (float)patchNum);
        }
    }
}

void GenCPUdata(int imgHeight, int imgWidth, unsigned char* data, int nChannels)
{
    float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
    for(unsigned int i = 0; i < imgHeight; i++)
    {
        for(unsigned int j = 0; j < imgWidth; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + width * i) * nChannels;
            // raw height at coordinate
            unsigned char y = texel[0];

            // vertex
            vertices.push_back( -imgHeight/2.0f + i);        // v.x
            vertices.push_back( (int)y * yScale - yShift); // v.y
            vertices.push_back( -imgWidth/2.0f + j);        // v.z
        }
    }
}


GLFWwindow* GLInit()
{

    // Initialize GLFW
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* windowi = glfwCreateWindow(800, 800, "Diplomska", NULL, NULL);
	if (windowi == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(windowi);
	gladLoadGL();
	glViewport(0, 0, 800, 800);

    GLint MaxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
    printf("Max supported patch vertices %d\n", MaxPatchVertices);

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glEnable(GL_DEPTH_TEST);
    return windowi;
}


void TextureSetup(int widthImg, int heightImg, Shader& shaderProgram, int* data)
{
    unsigned int texture;
    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (*data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    /* glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); */
    /* glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST ); */
    /* glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); */
    /* glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); */
    /* glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); */
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    GLuint textureToUni = glGetUniformLocation(shaderProgram.ID, "heightMap");
    glUniform1i(textureToUni, 0);
}

/* void printVert() */
/* { */
/*     for(int i = 0; i < vertices.size()/5; i++) */
/*     { */
/*         for( int z = 0; z < 5; z++) */
/*         { */
/*             std::cout << vertices[i*5+z] << ", "; */
/*         } */
/*         std::cout << "\n"; */
/*     } */
/* } */

int main()
{
    unsigned patchNum = 20;
    MakePatches(patchNum, 1000, 1000);
    /* printVert(); */

    int widthImg = 1000;
    int heightImg= 1000;
    Wave wave;
    int data[widthImg] [heightImg];
    wave.test(*data);

    window = GLInit();
	Shader shaderProgram("Libraries/Shaders/default.vert", "Libraries/Shaders/default.frag", "Libraries/Shaders/default.tesc", "Libraries/Shaders/default.tese");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, vertices.size()*sizeof(GLfloat));
    /* VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0); */
    /* VAO1.LinkAttribute(VBO1, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float))); */
    VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    VAO1.LinkAttribute(VBO1, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();

    TextureSetup(widthImg, heightImg, shaderProgram, *data);
    shaderProgram.Activate();

    float scale = 0.5f;
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));



	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window, scale);

        camera.Matrix(45.5f, 0.1f, 10000.0f, shaderProgram, "PV");
		camera.Inputs(window, scale);

        //just wireframe testing
        /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
		VAO1.Bind();
		glDrawArrays(GL_PATCHES, 0, 4*patchNum*patchNum); //maybe wrong

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

