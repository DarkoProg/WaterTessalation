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
 
const unsigned int width = 1000;
const unsigned int height = 1000;
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

std::vector<GLfloat> verticesTess;
std::vector<GLfloat> verticesCPU;
std::vector<GLuint> indices;
/* std::vector<unsigned int> indices; */

void MakePatches(int patchNum, int imgHeight, int imgWidth)
{
   /* verticesTess.clear(); */
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

template <size_t imgWidth, size_t imgHeight>
void GenCPUdata( int (&data)[imgWidth][imgHeight])
/* void GenCPUdata(int imgHeight, int imgWidth, int (&data)[rows][cols], int nChannels) */
{
    /* verticesCPU.clear(); */
    float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
    for(size_t i = 0; i < imgHeight; i++)
    {
        for(size_t j = 0; j < imgWidth; j++)
        {
            // retrieve texel for (i,j) tex coord
            /* int texel = (data + (j + width * i) * nChannels); */
            int y = data[i][j];
            // raw height at coordinate
            /* int y = texel; */

            // vertex
            verticesCPU.push_back( -(float)imgHeight/2.0f + i);        // v.x
            /* verticesCPU.push_back( y * yScale - yShift); // v.y */
            verticesCPU.push_back(y / 64.0f + 1.0f); // v.y
            verticesCPU.push_back( -(float)imgWidth/2.0f + j);        // v.z
        }
    }

    for(unsigned int i = 0; i < imgHeight-1; i++)       // for each row a.k.a. each strip
    {
        for(unsigned int j = 0; j < imgWidth; j++)      // for each column
        {
            for(unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(j + imgWidth * (i + k));
            }
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
    GLuint textureToUni = glGetUniformLocation(shaderProgramTess.ID, "heightMap");
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
    int widthImg = 1000;
    int heightImg= 1000;
    Wave wave;
    /* int data[widthImg] [heightImg]; */
    int data[1000] [1000];
    wave.test(*data);
    const unsigned int NUM_STRIPS = heightImg-1;
    const unsigned int NUM_VERTS_PER_STRIP = widthImg*2;
    bool gpu = true;
    unsigned patchNum = 20;
    MakePatches(patchNum, widthImg, heightImg);
    GenCPUdata(data);

    /* for(int i = 0; i < widthImg; i++) */
    /* { */
    /*     for(int j = 0; j < heightImg; j++) */
    /*     { */
    /*         std::cout << verticesCPU[i] << " "; */
    /*     } */
    /*     std::cout << "\n"; */
    /* } */

    /* std::cout << verticesCPU[0] << " " << verticesCPU[1] << " " << verticesCPU[2]; */

    window = GLInit();


    VAO VAOTess;
    VAOTess.Bind();
    VBO VBOTess(verticesTess, verticesTess.size()*sizeof(GLfloat));
    Shader shaderProgramTess("Libraries/Shaders/default.vert", "Libraries/Shaders/default.frag", "Libraries/Shaders/default.tesc", "Libraries/Shaders/default.tese");

    /* VAOTess.LinkAttribute(VBOTess, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0); */
    /* VAOTess.LinkAttribute(VBOTess, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float))); */
    VAOTess.LinkAttribute(VBOTess, 0, 3, GL_FLOAT, 5*sizeof(float), (void*)0);
    VAOTess.LinkAttribute(VBOTess, 1, 2, GL_FLOAT, 5*sizeof(float), (void*)(3*sizeof(float)));

    VAOTess.Unbind();
    VBOTess.Unbind();
    TextureSetup(widthImg, heightImg, shaderProgramTess, *data);


    VAO VAOCpu;
    VAOCpu.Bind();
    VBO VBOCpu(verticesCPU, verticesCPU.size()*sizeof(GLfloat));
    Shader shaderProgramCPU("Libraries/Shaders/cpu.vert", "Libraries/Shaders/cpu.frag");
    /* EBO EBOCpu(&indices, sizeof(indices)); */
    EBO EBOCpu(indices, indices.size()*sizeof(GLuint));

    VAOCpu.LinkAttribute(VBOCpu, 0, 3, GL_FLOAT, 3*sizeof(float), (void*)0);
    VAOCpu.Unbind();
    VBOCpu.Unbind();
    EBOCpu.Unbind();
    
    /* shaderProgramTess.Activate(); */

    float scale = 0.5f;
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        //just wireframe testing
        /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
        if(gpu)
        {
            shaderProgramTess.Activate(); 
            camera.Matrix(45.5f, 0.1f, 10000.0f, shaderProgramTess, "PV");
            camera.Inputs(window, scale);
            VAOTess.Bind();
            glDrawArrays(GL_PATCHES, 0, 4*patchNum*patchNum); //maybe wrong
        }
        else 
        {
            shaderProgramCPU.Activate();
            camera.Matrix(45.5f, 0.1f, 10000.0f, shaderProgramCPU, "PV");
            camera.Inputs(window, scale);
            VAOCpu.Bind();
        for(unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
            {
                glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                               NUM_VERTS_PER_STRIP, // number of indices to render
                               GL_UNSIGNED_INT,     // index data type
                               (void*)(sizeof(unsigned int)
                                         * NUM_VERTS_PER_STRIP
                                         * strip)); // offset to starting index
            }
            /* glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0); */
        }
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

