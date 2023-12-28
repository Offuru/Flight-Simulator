#include "Plane.h"

Plane::Plane(const glm::vec3& initialPos, Model* model)
{
	m_currPos = initialPos;
	m_plane = model;
}

void Plane::setPos(const glm::vec3& pos)
{
	m_currPos = pos;
}

glm::vec3 Plane::getPos() const
{
	return m_currPos;
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
