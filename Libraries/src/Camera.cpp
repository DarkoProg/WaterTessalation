#include "../include/Camera.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/trigonometric.hpp>


Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char* uniform)
{
	glm::mat4 V = glm::mat4(1.0f);
	glm::mat4 P = glm::mat4(1.0f);

	V = glm::lookAt(Position, Position + Orientation, Up);
	if(ProjectionType == 1)
	{
		P = glm::perspective(glm::radians(FOVdeg),(float) (width/height),nearPlane, farPlane);
	}
	else if(ProjectionType == 2)
	{
		float viewDis = PirPos.z + 1;
		P = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 50.0f); 
	}
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(P*V));
}
void Camera::Inputs(GLFWwindow *window, float &scale)
{
	//camera movment
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;	
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed *	-Orientation;	
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed *	-glm::normalize(glm::cross(Orientation, Up));	
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed *	glm::normalize(glm::cross(Orientation, Up));	
	}	
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		Position.y -= speed;	
	}
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		Position.y += speed;	
	}
	

	//camera rotation
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(Yrotation), glm::normalize(glm::cross(Orientation, Up)));	
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(-Yrotation), glm::normalize(glm::cross(Orientation, Up)));	

	}
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(Xrotation), Up);	

	}
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(-Xrotation), Up);	
	}

	//piramide scale
	if(glfwGetKey(window, GLFW_KEY_F1))
	{
		ProjectionType = 1;
	}
	if(glfwGetKey(window, GLFW_KEY_F2))
	{
		ProjectionType = 2;
	}

    /* std::cout << "x: " << Position.x << " y: " << Position.y << " z: " << Position.z << std::endl; */
}
