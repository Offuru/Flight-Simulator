// ViewOBJModel.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <Windows.h>
#include <locale>
#include <codecvt>

#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h> 

#include <glew/glew.h>

#include <glm/GLM.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glfw/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION 

#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "VAO.h"
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
void renderScene(Shader& shader);
void clean();

std::string currentPath;
std::vector<Model*> objects;


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

	glm::vec3 initialCameraPosition = glm::vec3(10.f, 1700.f, 20.f);
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, initialCameraPosition);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::vec3 lightPos(10000.0, 10000.0, 10000.0);

	wchar_t buffer[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, buffer);

	std::wstring executablePath(buffer);
	std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	currentPath = converter.to_bytes(wscurrentPath);
	//Shader modelShader("model_loading_nolight.vs", "model_loading_nolight.fs");
	//Shader modelShader("1.model_loading.vs", "1.model_loading.fs");
	Shader modelShader("sunlight.vs", "sunlight.fs");


	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	/*stbi_set_flip_vertically_on_load(true);*/



	std::string planeModelPath = (currentPath + "\\models\\SRTM\\untitled.obj");
	Model* SRTM = new Model(planeModelPath, false);

	std::string planeModelPath2 = (currentPath + "\\models\\Plane2\\untitled.obj");
	Model* planeModel2 = new Model(planeModelPath2, false);

	objects.push_back(SRTM);
	objects.push_back(planeModel2);
	

	Shader lightShader((currentPath + "\\Shaders\\Lamp.vs").c_str(), (currentPath + "\\Shaders\\Lamp.fs").c_str());
	LightSource light{ lightPos, 0.5f };
	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	// render loop

	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*lightPos.x = 0.5 * cos(glfwGetTime());
		lightPos.z = 0.5 * sin(glfwGetTime());*/

		glm::mat4 model = glm::translate(glm::mat4(1.0), lightPos);
		model = glm::scale(model, glm::vec3(0.05f));
		light.Render(lightShader, pCamera->GetProjectionMatrix(), pCamera->GetViewMatrix(), model);

		modelShader.use();
		renderScene(modelShader);
		//pt sunlight shader

		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	clean();
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
	pCamera->reshape(width, height);
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->reset(width, height);

	}
}

void renderScene(Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);

	Model* SRTM = objects[0];
	Model* planeModel2 = objects[1];

	shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	shader.setVec3("viewPos", pCamera->GetPosition());

	// light properties
	shader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
	shader.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
	shader.setVec3("light.specular", 0.0001f, 0.0001f, 0.0001f);

	// material properties
	shader.setValue("material.shininess", 0.0001f);

	// view/projection transformations
	shader.setMat4("projection", pCamera->GetProjectionMatrix());
	shader.setMat4("view", pCamera->GetViewMatrix());

	//plane
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.f, -6.f, -20.f));
	//model = glm::rotate(model, 3.14f, glm::vec3(0.f, 1.f, 0.f));

	shader.setMat4("model", model);
	SRTM->Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.f, 1700.f, 20.f));

	shader.setMat4("model", model);
	planeModel2->Draw(shader);
}

void clean()
{
	for (auto& obj : objects)
		delete obj;

	delete pCamera;
}
