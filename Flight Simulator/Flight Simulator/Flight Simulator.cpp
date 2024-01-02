// ViewOBJModel.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <Windows.h>
#include <locale>
#include <codecvt>

#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h> 

#include <glew/glew.h>
#define GLM_FORCE_CTOR_INIT 
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
#include "Plane.h"
#include "Object.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

Camera* pCamera = nullptr;
float Ka = 0.2f;
float lastProppelerRotation = 0.f;

// Time variables to manage the speed of the camera
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yOffset);
void processInput(GLFWwindow* window, LightSource& sun, LightSource& moon);
void renderScene(Shader& shader);
void clean();

std::string currentPath;
std::vector<Model*> objects;
unsigned int depthMap;
glm::vec3 lightPos(5.0f, 163000.f, -2.0);

Plane airplane({ 5.0f, 1630.f, -2.0 });
glm::vec3 m_currPos = { 5.0f, 1630.f, -2.0 };
std::vector<Object*> _objects;


int main() {
	wchar_t buffer[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, buffer);

	std::wstring executablePath(buffer);
	std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	currentPath = converter.to_bytes(wscurrentPath);

	// glfw: initialize and configure
	//glewExperimental = GL_TRUE;
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
	//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glm::vec3 initialCameraPosition = glm::vec3(5.0f, 1630.f, -2.0);
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, initialCameraPosition);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	Shader shadowMappingShader("ShadowMapping.vs", "ShadowMapping.fs");
	Shader shadowMappingDepthShader("shadow_mapping_depth.vs", "shadow_mapping_depth.fs");
	Shader lightShader("Lamp.vs", "Lamp.fs");
	std::string SRTMModelPath = (currentPath + "\\models\\SRTM\\untitled.obj");
	Model* SRTM = new Model(SRTMModelPath, false);


	std::string planeModelPath2 = (currentPath + "\\models\\warplane\\warplane.obj");
	Model* planeModel2 = new Model(planeModelPath2, false);

	Model* propeller = new Model(currentPath + "\\models\\propeller\\propeller.obj");

	airplane.setModel(planeModel2);

	objects.push_back(SRTM);
	objects.push_back(propeller);

	LightSource sun{ currentPath + "\\models\\sun\\untitled.obj" };
	LightSource moon{ currentPath + "\\models\\sun\\untitled.obj", true };


	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowMappingShader.use();
	shadowMappingShader.setInt("diffuseTexture", 0);
	shadowMappingShader.setInt("shadowMap", 1);

	glEnable(GL_CULL_FACE);

	Object* runway = new Object();
	runway->model = new Model(currentPath + "\\models\\Airport\\airport.obj");
	runway->position = glm::vec3(0, 1700, 0);

	Object* plane = new Object();
	plane->model = new Model(currentPath + "\\models\\Plane2\\untitled.obj");
	plane->position = glm::vec3(5.0f, 1730.f, -2.0);

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		airplane.movePlane(deltaTime);

		pCamera->setPosition(airplane.getPos());

		lightPos = sun.updateBySunPosition();
		moon.updateBySunPosition();

		//if sun is below the map it means that it is night so change Ka
		Ka = std::max(0.3f, 0.000001f * sun.getPos().y);
		Ka = std::min(1.0f, Ka);

		// input
		// -----
		processInput(window, sun, moon);
		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. render depth of scene to texture (from light's perspective)
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		glm::mat4 lightModelMatrix;


		float near_plane = 0.1f, far_plane = 1000.f;
		lightProjection = glm::ortho(-45.0f, 45.0f, -45.0f, 45.0f, near_plane, far_plane);
		glm::vec3 directionalLightPos = airplane.getPos() + glm::vec3(0.f, 5.f, 0.f);

		directionalLightPos.x = std::min(std::max(lightPos.x, -20.f), 20.f) + airplane.getPos().x;
		directionalLightPos.z = std::min(std::max(lightPos.z, -20.f), 20.f) + airplane.getPos().z;

		lightView = glm::lookAt(directionalLightPos, airplane.getPos() * glm::vec3(1.0f, 0.f, 1.0f), glm::vec3(0.0, 1.0, 0.0));


		lightSpaceMatrix = lightProjection * lightView;

		// render scene from light's point of view
		shadowMappingDepthShader.use();
		shadowMappingDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderScene(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. render scene as normal using the generated depth/shadow map 
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::translate(glm::mat4(1.0), sun.getPos());
		model = glm::scale(model, glm::vec3(30000.f));
		sun.Render(lightShader, pCamera->GetProjectionMatrix(), pCamera->GetViewMatrix(airplane), model);
		model = glm::translate(glm::mat4(1.0), moon.getPos());
		model = glm::scale(model, glm::vec3(30000.f));
		moon.Render(lightShader, pCamera->GetProjectionMatrix(), pCamera->GetViewMatrix(airplane), model);

		shadowMappingShader.use();
		glm::mat4 projection = pCamera->GetProjectionMatrix();
		glm::mat4 view = pCamera->GetViewMatrix(airplane, true);
		
		shadowMappingShader.setMat4("projection", projection);
		shadowMappingShader.setMat4("view", view);
		// set light uniforms
		shadowMappingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		shadowMappingShader.setVec3("viewPos", pCamera->GetPosition());
		shadowMappingShader.setVec3("light.position", lightPos);

		shadowMappingShader.setVec3("light.color", 1.f, 1.f, 1.f);
		shadowMappingShader.setVec3("light.ambient", Ka, Ka, Ka);
		shadowMappingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		shadowMappingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		shadowMappingShader.setFloat("light.constant", 1.0f);
		shadowMappingShader.setFloat("light.linear", FLT_MIN);
		shadowMappingShader.setFloat("light.quadratic", FLT_MIN);

		// material properties
		shadowMappingShader.setFloat("material.shininess", 32.0f);

		shadowMappingShader.setMat4("projection", projection);
		shadowMappingShader.setMat4("view", view);

		

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderScene(shadowMappingShader);

		runway->Render(shadowMappingShader); //textures are currently buggy, find another airport later
		plane->Render(shadowMappingShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
		//std::cout << lightPos.y << '\n';


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

void processInput(GLFWwindow* window, LightSource& sun, LightSource& moon)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		airplane.ProcessKeyboard(EPlaneMovementType::FORWARD);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		airplane.ProcessKeyboard(EPlaneMovementType::BACKWARD);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		airplane.ProcessKeyboard(EPlaneMovementType::LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		airplane.ProcessKeyboard(EPlaneMovementType::RIGHT);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		airplane.ProcessKeyboard(EPlaneMovementType::UP);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		airplane.ProcessKeyboard(EPlaneMovementType::DOWN);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		airplane.ProcessKeyboard(EPlaneMovementType::ROLLRIGHT);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		airplane.ProcessKeyboard(EPlaneMovementType::ROLLLEFT);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		sun.increment();
		moon.increment();
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->reset(width, height);

	}
}

void renderScene(Shader& shader)
{
	glm::mat4 model = glm::mat4();

	//srtm
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(5.0f, 0.f, -2.0));
	shader.setMat4("model", model);
	objects[0]->Draw(shader);

	//plane
	model = glm::mat4();
	
	model = glm::translate(model, airplane.getPos());
	

	model = glm::rotate(model, -glm::radians(90.f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, -glm::radians(airplane.getYaw()), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(airplane.getPitch()), glm::vec3(1, 0, 0));
	model = glm::rotate(model, -glm::radians(airplane.getRoll()), glm::vec3(0, 0, 1));

	model = glm::scale(model, glm::vec3(2.f));
	shader.setMat4("model", model);
	airplane.getModel()->Draw(shader);
	

	//propeller
	lastProppelerRotation += airplane.getSpeed() / 45;
	if (lastProppelerRotation > 360)
		lastProppelerRotation -= 360;

	
	model = glm::translate(model, glm::vec3(0.f, -0.22f, -4.7f));
	model = glm::rotate(model, glm::radians(lastProppelerRotation), glm::vec3(0, 0, 1));

	
	shader.setMat4("model", model);
	objects[1]->Draw(shader);
	//airplane.draw(shader);
}

void clean()
{
	for (auto& obj : objects)
		delete obj;

	delete pCamera;
}