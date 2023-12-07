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
#include "Camera.h"

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

Camera* pCamera = nullptr;

// Time variables to manage the speed of the camera
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yOffset);
void processInput(GLFWwindow* window);


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
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewInit();

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glm::vec3 initialCameraPosition = glm::vec3(0.0, 0.0, 3.0);
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, initialCameraPosition);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader shader("testShader.vs", "testShader.fs");

	Cube cube1{ glm::vec3{0.f,0.f,-1.f}, 1.f };
	Cube cube2{ glm::vec3{-1.f,1.f,-1.f}, 1.f };

	while (!glfwWindowShouldClose(window)) {
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		shader.setMat4("projection", pCamera->GetProjectionMatrix());
		shader.setMat4("view", pCamera->GetViewMatrix());

		shader.use();

		cube1.Render(glm::mat4(1.f));
		cube2.Render(glm::mat4(1.f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete pCamera;
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Ensure that the function only responds when a key is pressed or held down
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_W:
			pCamera->ProcessKeyboard(FORWARD, float(deltaTime));
			break;
		case GLFW_KEY_S:
			pCamera->ProcessKeyboard(BACKWARD, float(deltaTime));
			break;
		case GLFW_KEY_A:
			pCamera->ProcessKeyboard(LEFT, float(deltaTime));
			break;
		case GLFW_KEY_D:
			pCamera->ProcessKeyboard(RIGHT, float(deltaTime));
			break;
		case GLFW_KEY_Q:
			pCamera->ProcessKeyboard(DOWN, float(deltaTime));
			break;
		case GLFW_KEY_E:
			pCamera->ProcessKeyboard(UP, float(deltaTime));
			break;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
		std::cout << "\nUPPPPPPPPPP PRESSED IN PROCESSINPUT\n";
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);

	}
}