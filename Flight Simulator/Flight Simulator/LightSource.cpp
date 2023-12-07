#include "LightSource.h"

LightSource::LightSource(const glm::vec3& pos, GLfloat length) :
	m_lightVAO{ VAO() }, m_lightVBO{ VBO() }, m_lightEBO{ EBO() }
{
	m_vertices =
	{
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	/*m_vertices =
	{
		pos.x, pos.y, pos.z + length,								1.f, 0.f, 0.f,		0.f,0.f,
		pos.x + length, pos.y, pos.z + length,						1.f, 0.f, 0.f,		0.f,0.f,
		pos.x + length, pos.y + length, pos.z + length,				1.f, 0.f, 0.f,		0.f,0.f,
		pos.x , pos.y + length, pos.z + length,						1.f, 0.f, 0.f,		0.f,0.f,
		pos.x, pos.y, pos.z,										1.f, 0.f, 0.f,		0.f,0.f,
		pos.x + length, pos.y, pos.z,								1.f, 0.f, 0.f,		0.f,0.f,
		pos.x + length, pos.y + length, pos.z,						1.f, 0.f, 0.f,		0.f,0.f,
		pos.x, pos.y + length, pos.z,								1.f, 0.f, 0.f,		0.f,0.f,
	};

	m_indices =
	{
		0,1,2,
		0,2,3,
		1,5,6,
		1,6,2,
		5,4,7,
		5,7,6,
		4,0,3,
		4,3,7,
		0,5,1,
		0,4,5,
		3,2,6,
		3,6,7
	};*/

	m_lightVAO.Bind();
	m_lightVBO.Initialize(m_vertices);
	//m_lightEBO.Initialize(m_indices);

	m_lightVAO.LinkAttribute(m_lightVBO, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), nullptr);
	m_lightVAO.LinkAttribute(m_lightVBO, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	m_lightVAO.Unbind();
	m_lightVBO.Unbind();
	m_lightVAO.Unbind();

	m_lightPos = pos;
}

void LightSource::Render(Shader& lampShader, const glm::mat4& projection, const glm::mat4& view, glm::mat4& model)
{
	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);
	lampShader.setMat4("model", model);

	m_lightVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	m_lightVAO.Unbind();
}
