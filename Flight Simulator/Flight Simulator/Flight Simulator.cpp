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
#include "LightSource.h"

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
		   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		/*vertices =
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
			0,1,2,
			0,2,3,
			1,5,6,
			1,6,2,
			5,4,7,
			5,7,6,
			4,0,3,
			4,3,7,
			0,5,1,
			0,4,5,
			3,2,6,
			3,6,7
		};*/

		vao.Bind();
		vbo.Initialize(vertices);
		//ebo.Initialize(indices);
		vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
		vao.LinkAttribute(vbo, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		vao.Unbind();
		vbo.Unbind();
		vao.Unbind();
	}

	void Render(Shader& shader, const glm::mat4& model, Camera* pCamera, glm::vec3& lightPos, GLfloat& Ka, GLfloat& Kd, GLfloat& Ks, GLfloat& spec)
	{
		shader.use();
		shader.setVec3("objectColor", 0.5f, 1.0f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", pCamera->GetPosition());
		shader.setValue("Ka", Ka);
		shader.setValue("Kd", Kd);
		shader.setValue("Ks", Ks);
		shader.setValue("spec", spec);

		shader.setMat4("projection", pCamera->GetProjectionMatrix());
		shader.setMat4("view", pCamera->GetViewMatrix());

		shader.setMat4("model", model);

		vao.Bind();
		vbo.Bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.Unbind();
		//vao.Unbind();
	}

private:
	VAO vao;
	VBO vbo;
	EBO ebo;

	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;
};

Camera* pCamera = nullptr;
GLfloat Ka = 0.1;
GLfloat Kd = 0.5;
GLfloat Ks = 2;
GLfloat spec = 2;


// Time variables to manage the speed of the camera
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewInit();

	glEnable(GL_DEPTH_TEST);


	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glm::vec3 initialCameraPosition = glm::vec3(0.0, 0.0, 3.0);
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, initialCameraPosition);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::vec3 lightPos(3.0f, 0.0f, 0.0f);
	Cube cube{ glm::vec3{1.f,0.f,-1.f}, 1.f };
	LightSource light{ lightPos, 0.5f };

	Shader cubeShader("PhongLight.vs", "PhongLight.fs");
	Shader lightShader("Lamp.vs", "Lamp.fs");

	while (!glfwWindowShouldClose(window)) {
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		
		glm::mat4 model = glm::scale(glm::mat4(1.0), glm::vec3(3.0f));
		cube.Render(cubeShader, model, pCamera, lightPos, Ka, Kd, Ks, spec);

		model = glm::translate(glm::mat4(1.0), lightPos);
		model = glm::scale(model, glm::vec3(0.05f));
		light.Render(lightShader, pCamera->GetProjectionMatrix(), pCamera->GetViewMatrix(), model);

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
		
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	pCamera->Reshape(width, height);
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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
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
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Ka = std::min(1.f, Ka + 0.01f);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		Ka = std::max(0.f, Ka - 0.01f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Kd = std::min(1.f, Kd + 0.1f);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		Kd = std::max(0.f, Kd - 0.1f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Ks = std::min(1.f, Ks + 0.01f);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		Ks = std::max(0.f, Ks - 0.01f);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		spec = std::min(256.f, spec * 2);
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		spec = std::max(2.f, spec / 2);



	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);

	}
}