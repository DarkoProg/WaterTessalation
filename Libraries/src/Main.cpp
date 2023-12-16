#include "../include/glad.h"
#include "glm/gtx/string_cast.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
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
 
/* const unsigned int width = 1000; */
/* const unsigned int height = 1000; */
GLFWwindow* window; 


std::vector<GLfloat> verticesTess;
std::vector<GLfloat> verticesCPU;
std::vector<GLuint> indices;

void MakePatches(int patchNum, int imgHeight, int imgWidth)
{
   verticesTess.clear();
   for(int i = 0; i < patchNum; i++)
   {
        for(int j = 0; j < patchNum; j++)
        {

            //x
            //y
            //z
            //u
            //v
            verticesTess.push_back(-imgWidth/2.0f + imgWidth*j/(float)patchNum);
            verticesTess.push_back(0.0f);
            verticesTess.push_back(-imgHeight/2.0f + imgHeight*i/(float)patchNum);
            verticesTess.push_back(j / (float)patchNum);
            verticesTess.push_back(i / (float)patchNum);

            verticesTess.push_back(-imgWidth/2.0f + imgWidth*(j+1)/(float)patchNum);
            verticesTess.push_back(0.0f);
            verticesTess.push_back(-imgHeight/2.0f + imgHeight*i/(float)patchNum);
            verticesTess.push_back((j+1) / (float)patchNum);
            verticesTess.push_back(i / (float)patchNum);

            verticesTess.push_back(-imgWidth/2.0f + imgWidth*(j+1)/(float)patchNum);
            verticesTess.push_back(0.0f);
            verticesTess.push_back(-imgHeight/2.0f + imgHeight*(i+1)/(float)patchNum);
            verticesTess.push_back((j+1) / (float)patchNum);
            verticesTess.push_back((i+1) / (float)patchNum);
                                                         
            verticesTess.push_back(-imgWidth/2.0f + imgWidth*j/(float)patchNum);
            verticesTess.push_back(0.0f);
            verticesTess.push_back(-imgHeight/2.0f + imgHeight*(i+1)/(float)patchNum);
            verticesTess.push_back(j / (float)patchNum);
            verticesTess.push_back((i+1) / (float)patchNum);
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

	GLFWwindow* windowi = glfwCreateWindow(1920, 1080, "Diplomska", NULL, NULL);
	if (windowi == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(windowi);
	gladLoadGL();
	glViewport(0, 0, 1920, 1080);

    GLint MaxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
    printf("Max supported patch vertices %d\n", MaxPatchVertices);

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glEnable(GL_DEPTH_TEST);
    return windowi;
}


void TextureSetup(int widthImg, int heightImg, Shader& shaderProgramTess, int* data)
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
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    GLuint textureToUni = glGetUniformLocation(shaderProgramTess.ID, "heightMap");
    glUniform1i(textureToUni, 0);
}

int main()
{
    int widthImg = 500;
    int heightImg= 500;
    Wave wave(widthImg, heightImg);
    int data[1000] [1000];
    std::cout << "before wave data";
    wave.GenWave(*data, 0);
    unsigned patchNum = 16;
    std::cout << "before arrays";
    MakePatches(patchNum, 500, 500);

    window = GLInit();


    VAO VAOTess;
    VAOTess.Bind();
    VBO VBOTess(verticesTess, verticesTess.size()*sizeof(GLfloat));
    Shader shaderProgramTess("Libraries/Shaders/default.vert", "Libraries/Shaders/default.frag", "Libraries/Shaders/default.tesc", "Libraries/Shaders/default.tese");

    VAOTess.LinkAttribute(VBOTess, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    VAOTess.LinkAttribute(VBOTess, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));

    VAOTess.Unbind();
    VBOTess.Unbind();
    /* TextureSetup(widthImg, heightImg, shaderProgramTess, *data); */

    /* shaderProgramTess.Activate(); */

    float scale = 0.5f;
    Camera camera(1920, 1080, glm::vec3(-500.0f, 50.0f, 500.0f));

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    float time = 0;

    std::cout << "before loop";
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        //just wireframe testing
        /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
        shaderProgramTess.Activate(); 
        camera.Matrix(45.5f, 0.1f, 10000.0f, shaderProgramTess, "PV");
        camera.Inputs(window, scale);
        VAOTess.Bind();
        glDrawArrays(GL_PATCHES, 0, 4*patchNum*patchNum); //maybe wrong
		glfwSwapBuffers(window);
		glfwPollEvents();

        
        /* std::cout << "x: " << camera.Position.x << " y: " << camera.Position.y << " z: " << camera.Position.z << std::endl; */
	}

	VAOTess.Delete();
	VBOTess.Delete();
	shaderProgramTess.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

