#include <stdlib.h> 
#include <stdio.h>
#include <math.h> 
#include <vector>
#include <array>

#include <glew.h>

#include <glm/GLM.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Shader.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Cube
{
public:
	Cube(const glm::vec3& pos, GLfloat length) :
		vao{ VAO{} }, vbo{ VBO{} }, ebo{ EBO{} }
	{
		vertices =
		{
			pos.x, pos.y, pos.z,								1.f, 0.f, 0.f,		0.f,0.f,
			pos.x + length, pos.y, pos.z,						1.f, 0.f, 0.f,		0.f,0.f,
			pos.x, pos.y, pos.z + length,						1.f, 0.f, 0.f,		0.f,0.f,
			pos.x + length, pos.y, pos.z + length,				1.f, 0.f, 0.f,		0.f,0.f,
			pos.x, pos.y - length , pos.z,						1.f, 0.f, 0.f,		0.f,0.f,
			pos.x + length, pos.y - length, pos.z,				1.f, 0.f, 0.f,		0.f,0.f,
			pos.x, pos.y - length, pos.z + length,				1.f, 0.f, 0.f,		0.f,0.f,
			pos.x + length, pos.y - length, pos.z + length,		1.f, 0.f, 0.f,		0.f,0.f,
		};

		indices =
		{
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			0, 1, 5,
			0, 5, 4,
			0, 7, 4,
			0, 3, 7,
			2, 1, 5,
			2, 5, 6,
			3, 2, 6,
			3, 6, 7
		};

		vao.Bind();
		vbo.Initialize(vertices);
		ebo.Initialize(indices);
		vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), nullptr);
		vao.LinkAttribute(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		vao.Unbind();
		vbo.Unbind();
		vao.Unbind();
	}

	void Render(const glm::mat4& model)
	{
		vao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.Unbind();
		vbo.Unbind();
		vao.Unbind();
	}

private:
	VAO vao;
	VBO vbo;
	EBO ebo;

	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;
};

int main() {

	// glfw: initialize and configure
	glewExperimental = GL_TRUE;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Flight Simulator", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewInit();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Shader shader("testShader.vs", "testShader.fs");

	Cube cube1{ glm::vec3{0.f,0.f,-1.f}, 1.f };
	Cube cube2{ glm::vec3{-1.f,1.f,-1.f}, 1.f };

	GLuint testID, testID2;

	while (!glfwWindowShouldClose(window)) {

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();

		cube1.Render(glm::mat4(1.f));
		cube2.Render(glm::mat4(1.f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}