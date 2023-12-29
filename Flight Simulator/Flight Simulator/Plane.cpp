#include "Plane.h"

Plane::Plane(const glm::vec3& initialPos, Model* model)
{
	m_position = initialPos;
	m_plane = model;
}

void Plane::setPos(const glm::vec3& pos)
{
	m_position = pos;
}

glm::vec3 Plane::getPos() const
{
	return m_position;
}

void Plane::setModel(Model* model)
{
	m_plane = model;
}

Model* Plane::getModel()
{
	return m_plane;
}

void Plane::draw(Shader& shader)
{
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, glm::vec3(5.0f, 1630.f, -2.0));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	m_plane->Draw(shader);
}

void Plane::ProcessKeyboard(EPlaneMovementType direction)
{
	switch (direction)
	{
	case EPlaneMovementType::FORWARD:
		currentSpeed = std::min(currentSpeed + 0.4f, 100.f);
		break;
	case EPlaneMovementType::BACKWARD:
		currentSpeed = std::max(currentSpeed - 0.4f, 0.f);
		break;
	case EPlaneMovementType::RIGHT:
		if (currentSpeed > 0.1f)
		{
			m_yaw += 0.2f;
			if (m_yaw >= 360)
				m_yaw -= 360;
			
			currentYaw = 0.2f;
		}
		break;
	case EPlaneMovementType::LEFT:
		if (currentSpeed > 0.1f)
		{
			m_yaw -= 0.2f;
			if (m_yaw < 0)
				m_yaw = 360;

			currentYaw = -0.2f;
		}
		break;

	case EPlaneMovementType::UP:
		m_pitch = std::min(m_pitch + 0.1f, 45.f);
		break;
	case EPlaneMovementType::DOWN:
		m_pitch = std::max(m_pitch - 0.1f, -45.f);
		break;

	}

	UpdatePlaneVectors();
}

float Plane::getYaw() const
{
	return m_yaw;
}

void Plane::movePlane(float deltaTime)
{
	float velocity = (float)(deltaTime);

	m_position += m_forward * velocity * currentSpeed;

	m_position += m_right * velocity * currentYaw;

	m_position += m_up * velocity * m_pitch;

	std::cout << m_forward.x << " " << m_forward.y << " " << m_forward.z << '\n';
	
	currentSpeed = std::max(currentSpeed - 0.1f, 0.f);
	currentYaw = 0.f;

	if (currentSpeed)
		m_pitch -= 0.01f;
}

void Plane::UpdatePlaneVectors()
{
	m_forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_forward.y = sin(glm::radians(m_pitch));
	m_forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_forward = glm::normalize(m_forward);

	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_forward));
}

float Plane::getPitch() const
{
	return m_pitch;
}
