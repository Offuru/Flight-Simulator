#pragma once
#include "Model.h"
#include "glfw/glfw3.h"

enum class EPlaneMovementType
{
	UNKNOWN,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ROLLLEFT,
	ROLLRIGHT
};

class Plane
{
public:
	Plane(const glm::vec3& initialPos, Model* model = nullptr);
	void setPos(const glm::vec3& pos);
	glm::vec3 getPos() const;

	void setModel(Model* model);
	Model* getModel();

	void draw(Shader& shader);


	void ProcessKeyboard(EPlaneMovementType direction);

	void movePlane(float deltaTime);

	float getYaw() const;

	float getPitch() const;

	float getRoll() const;

	float getSpeed() const;
private:
	void UpdatePlaneVectors();


	Model* m_plane;
	glm::vec3 m_position;
	glm::vec3 m_forward{ 0.f, 0.f, 1.f };
	glm::vec3 m_right{ 0.f, 1.f, 0.f };
	glm::vec3 m_up{ 0.f, 0.f, 1.f };
	glm::vec3 m_worldUp = { 0.f, 1.f, 0.f };

	float m_yaw = 90.0f;
	float m_pitch = 0.0f;
	float m_roll = 0.0f;
	const float rollMoveRight = 0.5f;

	float currentSpeed = 0.0f;
	float currentYaw = 0.0f;

	bool grounded;

	float takeoffTimer;
	float takeoffCooldown = 3.f;
};

