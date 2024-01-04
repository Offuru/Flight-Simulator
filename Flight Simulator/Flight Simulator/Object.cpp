#include "Object.h"

Object::Object(Model* model, glm::vec3 position, glm::vec3 scale, glm::vec3 up, glm::vec3 right, glm::vec3 forward)
{
	this->model = model;
	this->position = position;
	this->scale = scale;
	this->up = up;
	this->right = right;
	this->forward = forward;
}

Object::~Object()
{
	delete model;
}

void Object::Render(Shader& shader) const
{
	glm::mat4 modelMatrix(1);

	modelMatrix = glm::translate(modelMatrix, position);
	glm::mat4 rotation(1);

	rotation[0][0] = right.x; rotation[0][1] = right.y; rotation[0][2] = right.z;
	rotation[1][0] = up.x; rotation[1][1] = up.y; rotation[1][2] = up.z;
	rotation[2][0] = forward.x; rotation[2][1] = forward.y; rotation[2][2] = forward.z;

	modelMatrix = rotation * modelMatrix;

	modelMatrix = glm::scale(modelMatrix, scale);

	shader.setMat4("model", modelMatrix);

	model->Draw(shader);
}
