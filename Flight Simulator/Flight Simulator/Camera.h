#pragma once

#include <math.h>
#include <glm/GLM.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glew/glew.h>
#include <iostream>

#include <glfw/glfw3.h>
#include "Plane.h"

enum ECameraMovementType
{
	UNKNOWN,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	Camera(const int width, const int height, const glm::vec3& position);

	void set(const int width, const int height, const glm::vec3& position);
	void reset(const int width, const int height);
	void reshape(int windowWidth, int windowHeight);

	const glm::mat4 GetViewMatrix(Plane& plane, bool thirdPerson = true);
	const glm::vec3 GetPosition() const;
	const glm::mat4 GetProjectionMatrix() const;

	void ProcessKeyboard(ECameraMovementType direction, float deltaTime);
	void MouseControl(float xPos, float yPos);
	void ProcessMouseScroll(float yOffset);

	float getFoV() const;

	void setPosition(const glm::vec3& position);

private:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateCameraVectors();

	const float zNEAR = 0.1f;
	const float zFAR = 5000000.f;
	const float YAW = 0.0f;
	const float PITCH = 0.0f;
	const float FOV = 45.0f;
	glm::vec3 startPosition;

	const float cameraSpeedFactor = 2.5f;
	const float mouseSensitivity = 0.1f;
	const float cameraSpeed = 50.f;

	float lastMouseMovement = (float)glfwGetTime();

	float zNear;
	float zFar;
	float FoVy;
	int width;
	int height;
	bool isPerspective;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	bool bFirstMouseMove = true;
	float lastX = 0.f, lastY = 0.f;
};

