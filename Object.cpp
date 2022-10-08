#include "Object.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>


void Object::prepM() {
  const glm::mat4 startMatrix = glm::mat4(1.0f);
  glm::mat4 T = glm::translate(startMatrix, glm::vec3(x, y, z));
    glm::mat4 R = rot;
    glm::mat4 S = glm::scale(startMatrix, glm::vec3(scale, scale, scale));
    M = T * R * S;
}

void Object::control(GLFWwindow *window)
{
	//piramide scale
	if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		if(scale >= 0.3f)
		{
			scale -= 0.1f;
		}
	}
	if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		if(scale <= 1.5f)
		{
			scale += 0.1f;
		}
	}

	//piramid movment
	if(glfwGetKey(window, GLFW_KEY_I))
	{
		z -= 0.1f;
	}
	if(glfwGetKey(window, GLFW_KEY_K))
	{
		z += 0.1f;
	}
	if(glfwGetKey(window, GLFW_KEY_J))
	{
		x -= 0.1f;
	}
	if(glfwGetKey(window, GLFW_KEY_L))
	{
		x += 0.1f;
	}
	if(glfwGetKey(window, GLFW_KEY_U))
	{
		y -= 0.1f;
	}
	if(glfwGetKey(window, GLFW_KEY_O))
	{
		y += 0.1f;
	}

	//piramid rotation
	if(glfwGetKey(window, GLFW_KEY_T))
	{
		rot = glm::rotate(rot, glm::radians(rotSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if(glfwGetKey(window, GLFW_KEY_G))
	{
		rot = glm::rotate(rot, glm::radians(-rotSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if(glfwGetKey(window, GLFW_KEY_F))
	{
		rot = glm::rotate(rot, glm::radians(rotSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if(glfwGetKey(window, GLFW_KEY_H))
	{
		rot = glm::rotate(rot, glm::radians(-rotSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if(glfwGetKey(window, GLFW_KEY_R))
	{
		rot = glm::rotate(rot, glm::radians(rotSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if(glfwGetKey(window, GLFW_KEY_Y))
	{
		rot = glm::rotate(rot, glm::radians(-rotSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}
