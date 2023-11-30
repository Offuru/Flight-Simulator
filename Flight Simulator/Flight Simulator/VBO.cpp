#include "VBO.h"

VBO::VBO()
{
	glGenBuffers(1, &m_ID);
}

void VBO::Initialize(std::vector<GLfloat>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &m_ID);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint VBO::getID() const
{
	return m_ID;
}
