#include "Plane.h"

Plane::Plane(const glm::vec3& initialPos, Model* model)
{
	m_position = initialPos;
	m_plane = model;

	UpdatePlaneVectors();
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
		currentSpeed = std::max(currentSpeed - 0.1f, 0.f);
		break;
	case EPlaneMovementType::RIGHT:
		if (currentSpeed > 0.1f)
		{
			m_yaw += 0.1f;
			if (m_yaw >= 360)
				m_yaw -= 360;
			
			currentYaw = 0.1f;
		}
		break;
	case EPlaneMovementType::LEFT:
		if (currentSpeed > 0.1f)
		{
			m_yaw -= 0.1f;
			if (m_yaw < 0)
				m_yaw = 360;

			currentYaw = -0.1f;
		}
		break;

	case EPlaneMovementType::UP:
		if (currentSpeed > 0.1f)
			m_pitch = std::min(m_pitch + 0.03f, 45.f);
		break;
	case EPlaneMovementType::DOWN:
		if (currentSpeed > 0.1f)
			m_pitch = std::max(m_pitch - 0.03f, -45.f);
		break;
	
	case EPlaneMovementType::ROLLRIGHT:
		if (currentSpeed > 0.1f)
			m_roll += 0.5f;
		if (m_roll >= 360)
			m_roll -= 360;
		break;
	case EPlaneMovementType::ROLLLEFT:
		if (currentSpeed > 0.1f)
			m_roll -= 0.5f;
		if (m_roll <= -360)
			m_roll -= 360;
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

	if (currentSpeed > 0.0f)
	{
		m_position += m_up * velocity * m_pitch;
		m_pitch -= 0.004f;


		if (m_roll != 0.f)
		{
			m_position += m_right * velocity * rollMoveRight * m_roll;

			if ((m_roll >= 0 && m_roll <= 180.f) ||
				(m_roll >= -360.f && m_roll <= -180.0f))
				m_roll -= 0.1f, m_yaw += 0.03;
			else m_roll += 0.1f, m_yaw -= 0.03;

			if (m_roll >= 360 || m_roll <= -360)
				m_roll = 0;

			if (std::abs(m_roll) < 0.2f)
				m_roll = 0.0f;
		}
	}
	
	currentSpeed = std::max(currentSpeed - 0.01f, 0.f);
	currentYaw = 0.f;
}

void Plane::UpdatePlaneVectors()
{
	float x = glm::radians(m_yaw);
	float y = glm::radians(m_pitch);
	float z = glm::radians(m_roll);

	m_forward.x = cos(x) * cos(y);
	m_forward.y = sin(y);
	m_forward.z = sin(x) * cos(y);
	m_forward = glm::normalize(m_forward);

	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_forward));
}

float Plane::getPitch() const
{
	return m_pitch;
}

float Plane::getRoll() const
{
	return m_roll;
}

float Plane::getSpeed() const
{
	return currentSpeed;
}
