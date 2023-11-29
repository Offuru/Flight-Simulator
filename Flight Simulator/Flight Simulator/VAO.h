#pragma once

#include <glew.h>
#include "VBO.h"
#include "EBO.h"

class VAO
{
public:

	VAO();
	~VAO();

	void LinkAttribute(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();

	GLuint& getID();
private:

	GLuint m_ID;
};

