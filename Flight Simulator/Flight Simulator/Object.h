#pragma once
#include "Model.h"

class Object
{
public:
	Object(Model* model = nullptr, glm::vec3 position = glm::vec3(0),
		glm::vec3 scale = glm::vec3(1), glm::vec3 up = glm::vec3(0, 1, 0), glm::vec3 right = glm::vec3(1, 0, 0),
		glm::vec3 forward = glm::vec3(0, 0, 1));

	void Render(Shader& shader) const;

	Model* model;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;
};

