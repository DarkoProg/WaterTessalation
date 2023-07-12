#include "../include/glad.h"
/* #include "Libraries/include/imgui/imgui.h" */
/* #include "Libraries/include/imgui/imgui_impl_glfw.h" */
/* #include "Libraries/include/imgui/imgui_impl_opengl3.h" */
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
 

/* #define STB_IMAGE_IMPLEMENTATION */
/* #include "Libraries/include/stb/stb_image.h" */

const unsigned int width = 800;
const unsigned int height = 800;

/* // Vertices coordinates */
/* GLfloat vertices[] = */
/* { */
/* 	-0.5f, -0.5f , 0.0f, 0.0f, 0.0f, // Lower left corner */
/* 	 0.5f, -0.5f , 0.0f, 1.0f, 0.0f, // Lower right corner */
/* 	-0.5f,  0.5f , 0.0f, 0.0f, 1.0f,// Upper left */
/* 	 0.5f,  0.5f , 0.0f, 1.0f, 1.0f// Upper right */
/* }; */

/* // Indices for vertices order */
/* GLuint indices[] = */
/* { */
/*     3, 2, 0, 1 */
/* 	/1* 0, 1, 2, // Lower left triangle *1/ */
/* 	/1* 1, 2, 3, // Lower right triangle *1/ */
/*     /1* 0, 2, 3, *1/ */
/*     /1* 0, 1, 2 *1/ */
/* }; */

std::vector<float> vertices;

void GenVertices(unsigned numberOfPatches)
{
    for(unsigned i = 0; i < numberOfPatches; i++)
    {
        for (unsigned j = 0; j < numberOfPatches; j++)
        {
            /*  vertice stucture
             *
             *  x
             *  y
             *  z
             *  u
             *  v
             *
             * */
             vertices.push_back((-(float)width/2.0f + (float)width*i/(float)numberOfPatches)); 
             vertices.push_back(0.0f);
             vertices.push_back((-(float)height/2.0f + (float)height*j/(float)numberOfPatches)); 
             vertices.push_back(i / (float)numberOfPatches);
             vertices.push_back(j / (float)numberOfPatches);

             vertices.push_back((-(float)width/2.0f + (float)width*(i+1)/(float)numberOfPatches)); 
             vertices.push_back(0.0f);
             vertices.push_back((-(float)height/2.0f + (float)height*j/(float)numberOfPatches)); 
             vertices.push_back((i+1) / (float)numberOfPatches);
             vertices.push_back(j / (float)numberOfPatches);

             vertices.push_back((-(float)width/2.0f + (float)width*i/(float)numberOfPatches)); 
             vertices.push_back(0.0f);
             vertices.push_back((-(float)height/2.0f + (float)height*(j+1)/(float)numberOfPatches)); 
             vertices.push_back(i / (float)numberOfPatches);
             vertices.push_back((j+1) / (float)numberOfPatches);

             vertices.push_back((-(float)width/2.0f + (float)width*(i+1)/(float)numberOfPatches)); 
             vertices.push_back(0.0f);
             vertices.push_back((-(float)height/2.0f + (float)height*(j+1)/(float)numberOfPatches)); 
             vertices.push_back((i+1) / (float)numberOfPatches);
             vertices.push_back((j+1) / (float)numberOfPatches);
        }
    }

}

int main()
{
    float scale = 1.0f;
    unsigned numberOfPatches = 1;
    GenVertices(numberOfPatches);

    /* for(int i = 0; i < vertices.size(); i++) */
    /* { */
    /*     std::cout << vertices[i] << std::endl; */
    /* } */

    Wave wave;
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
	GLFWwindow* window = glfwCreateWindow(800, 800, "Diplomska", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);



	// Generates Shader object using shaders defualt.vert and default.frag
	/* Shader shaderProgram("../Shaders/default.vert", "../Shaders/default.frag", "../Shaders/default.tcs", "../Shaders/default.tes"); */
	Shader shaderProgram("Libraries/Shaders/default.vert", "Libraries/Shaders/default.frag", "Libraries/Shaders/default.tesc", "Libraries/Shaders/default.tese");



    GLint MaxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
    printf("Max supported patch vertices %d\n", MaxPatchVertices);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    //image placeholder load
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	/* int width = 1000; */
    /* int height = 1000; */
    int widthImg = 1000;
    int heightImg= 1000;
	/* unsigned char* data = stbi_load("~/diplomska/ShaderTest/image.png", &widthImg, &heightImg, &numColCh, 0); */
    Wave Wave(widthImg, heightImg);
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
	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	/* EBO EBO1(indices, sizeof(indices)); */


	// Links VBO to VAO
	/* VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void*)0); */
    VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    /* VAO1.LinkAttribute(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float))); */
    VAO1.LinkAttribute(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	/* EBO1.Unbind(); */

    GLuint textureToUni = glGetUniformLocation(shaderProgram.ID, "gSampler");
    shaderProgram.Activate();
    glUniform1i(textureToUni, 0);

    /* Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)); */


    /* glm::mat4 projection = glm::perspectiveFov(glm::radians(45.5f), (float)width, (float) height, 0.1f, 100.0f); */

    /* glm::mat4 view = glm::lookAt(glm::vec3(1,1,1), glm::vec3(0,0,0), glm::vec3(0,1,0)); */

    glm::mat4 model = glm::mat4(1.0f);


    Camera camera(width, height, glm::vec3(1,1,1));
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		// Bind the VAO so OpenGL knows to use it
        
        GLuint modelID = glGetUniformLocation(shaderProgram.ID, "model");
        GLuint viewID = glGetUniformLocation(shaderProgram.ID, "view");
        GLuint projectionID = glGetUniformLocation(shaderProgram.ID, "projection");


        camera.Inputs(window, scale);

        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
        /* glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]); */
        /* glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]); */
        camera.Matrix(45.5f, 0.1f, 100.0f, shaderProgram, "PV");

		// Handles camera inputs
        float scale = 0.5f;
		/* camera.Inputs(window, scale); */
		// Updates and exports the camera matrix to the Vertex Shader
		/* camera.Matrix(0.0f, 0.0f, 100.0f, shaderProgram, "camMatrix"); */

        //just wirefram testing
        /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

        /* glBindTexture(GL_TEXTURE_2D, texture); */
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		/* glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0); */
		glDrawArrays(GL_PATCHES, 0, 4*4*numberOfPatches); //maybe wrong

        /* glPolygonMode(GL_FRONT, GL_FILL); */
        /* glPolygonMode(GL_BACK, GL_FILL); */
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

        /* std::cout << "x: " << camera.Position.x << " y: " << camera.Position.y << " z: " << camera.Position.z << std::endl; */
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	/* EBO1.Delete(); */
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

