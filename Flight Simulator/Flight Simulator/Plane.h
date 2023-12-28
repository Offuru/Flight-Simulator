#pragma once
#include "Model.h"


class Plane
{
public:
	Plane(const glm::vec3& initialPos, Model* model = nullptr);
	void setPos(const glm::vec3& pos);
	glm::vec3 getPos() const;

	void setModel(Model* model);
	Model* getModel();

	void draw(Shader& shader);


private:
	Model* m_plane;
	glm::vec3 m_currPos;
};

