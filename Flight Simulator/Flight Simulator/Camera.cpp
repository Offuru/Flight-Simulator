#include "Camera.h"

Camera::Camera(const int width, const int height, const glm::vec3& position)
{
	startPosition = position;
	set(width, height, position);
}

void Camera::set(const int width, const int height, const glm::vec3& position)
{
	isPerspective = true;
	yaw = YAW;
	pitch = PITCH;
	FoVy = FOV;
	this->width = width;
	this->height = height;
	this->position = position;
	zNear = zNEAR;
	zFar = zFAR;
	worldUp = glm::vec3(0.f, 1.f, 0.f);

	lastX = width / 2.f;
	lastY = height / 2.f;
	bFirstMouseMove = true;

	UpdateCameraVectors();
}

void Camera::reset(const int width, const int height)
{
	set(width, height, startPosition);
}

void Camera::reshape(int windowWidth, int windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	glViewport(0, 0, width, height);
}

const glm::mat4 Camera::GetViewMatrix(Plane& plane, bool thirdPerson)
{
	if (!thirdPerson)
		return glm::lookAt(position, position + forward, up);

	float currentTime = (float)glfwGetTime();

	float planeYaw = plane.getYaw() - 90.f;
	float planePitch = plane.getPitch();

	if (currentTime - lastMouseMovement >= 2.f)
	{
		//reverting camera pos
		if (yaw != planeYaw)
		{
			if ((yaw >= planeYaw && yaw <= planeYaw + 180.f) ||
				(yaw >= planeYaw - 359.999f && yaw <= planeYaw - 180.0f))
				yaw -= 0.1f;
			else yaw += 0.1f;

			if (yaw >= planeYaw + 360)
				yaw -= 360;
			if (yaw <= planeYaw - 360)
				yaw += 360;

			if (std::abs(yaw - planeYaw) < 0.5 || std::abs(yaw + planeYaw + 180) < 0.5)
				yaw = planeYaw;
		}
		std::cout << planeYaw << " " << yaw << '\n';

		if (pitch != -planePitch)
		{
			if (pitch >= -planePitch)
				pitch -= 0.1f;
			else
				pitch += 0.1f;

			if ((int)pitch == (int)-planePitch)
				pitch = -planePitch;
		}
	}
	float distance = 25.0f;
	float x = distance * cos(glm::radians(pitch)) * sin(glm::radians(-yaw));
	float y = distance * sin(glm::radians(pitch));
	float z = distance * cos(glm::radians(pitch)) * cos(glm::radians(yaw));


	return glm::lookAt(plane.getPos() + glm::vec3(-x, y, -z) + glm::vec3(0.f, 10.f, 0.f), plane.getPos() + glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.0f, 0.1f, 0.0f));
}

const glm::vec3 Camera::GetPosition() const
{
	return position;
}

const glm::mat4 Camera::GetProjectionMatrix() const
{
	glm::mat4 proj = glm::mat4(1.f);

	if (isPerspective)
	{
		float aspectRatio = (float)width / height;
		proj = glm::perspective(glm::radians(FoVy), aspectRatio, zNear, zFar);
	}
	else
	{
		float scaleFactor = 2000.f;

		proj = glm::ortho<float>(-width / scaleFactor, width / scaleFactor,
			-height / scaleFactor, height / scaleFactor);
	}

	return proj;
}

void Camera::ProcessKeyboard(ECameraMovementType direction, float deltaTime)
{
	float velocity = (float)(cameraSpeedFactor * deltaTime);

	switch (direction)
	{
	case FORWARD:
		position += forward * velocity * cameraSpeed;
		break;
	case BACKWARD:
		position -= forward * velocity * cameraSpeed;
		break;
	case LEFT:
		position -= right * velocity * cameraSpeed;
		break;
	case RIGHT:
		position += right * velocity * cameraSpeed;
		break;
	case UP:
		position += up * velocity * cameraSpeed;
		break;
	case DOWN:
		position -= up * velocity * cameraSpeed;
		break;
	default:
		break;
	}
}

void Camera::MouseControl(float xPos, float yPos)
{
	if (bFirstMouseMove)
	{
		lastX = xPos;
		lastY = yPos;
		bFirstMouseMove = false;
	}

	float dx = xPos - lastX;
	float dy = yPos - lastY;
	lastX = xPos;
	lastY = yPos;

	if (fabs(dx) <= 1e-6 && fabs(dy) <= 1e-6)
		return;

	dx *= mouseSensitivity;
	dy *= mouseSensitivity;

	lastMouseMovement = (float)glfwGetTime();

	ProcessMouseMovement(dx, dy);
}

void Camera::ProcessMouseScroll(float yOffset)
{
	if (FoVy >= 1.f && FoVy <= 90.f)
		FoVy -= yOffset;

	if (FoVy <= 1.f)
		FoVy = 1.f;
	if (FoVy >= 90.f)
		FoVy = 90.f;
}

float Camera::getFoV() const
{
	return FoVy;
}

void Camera::setPosition(const glm::vec3& position)
{
	this->position = position;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	yaw += xOffset;
	pitch -= yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.f)
			pitch = 89.f;
		if (pitch < -89.f)
			pitch = -89.f;
	}

	if (yaw >= 360)
		yaw -= 360;
	else if (yaw <= -360)
		yaw += 360;
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}
