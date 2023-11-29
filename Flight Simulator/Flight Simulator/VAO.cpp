#include "VAO.h"
#include <iostream>

VAO::VAO()
{
	glGenVertexArrays(1, &m_ID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VAO::LinkAttribute(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(m_ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

GLuint& VAO::getID()
{
	return m_ID;
}





