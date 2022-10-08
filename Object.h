#ifndef Object_CLASS_H
#define Object_CLASS_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#include <cstdio>
#include <cstring>
#include <string>

class Object
{
	public:
		std::vector<float> obj;
		int vertNum;

		int swap = 0;

		glm::mat4 M;
		glm::mat4 rot = glm::mat4(1.0f);
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		float rotSpeed = 0.8f;

		void prepM();
		void control(GLFWwindow *window);
};
#endif
