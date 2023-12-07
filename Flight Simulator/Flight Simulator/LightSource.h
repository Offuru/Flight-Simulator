#pragma once
#include <string>

#include <glew.h>
#include <glm/GLM.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfw3.h>

#include "Shader.h"
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"

class LightSource
{
public:
	LightSource(const glm::vec3& pos, GLfloat length);
	~LightSource() = default;

	void Render(Shader& lampShader, const glm::mat4& projection, const glm::mat4& view, glm::mat4& model);
private:
	VAO m_lightVAO;
	VBO m_lightVBO;
	EBO m_lightEBO;

	glm::vec3 m_lightPos;
	std::vector<GLuint> m_indices;
	std::vector<GLfloat> m_vertices;
};

