#pragma once

#include <glew.h>
#include <vector>

class EBO
{
public:

	EBO() = default;
	EBO(std::vector<GLuint> indices);
	~EBO();

	void Bind();
	void Unbind();

	
private:
	GLuint m_ID;
	
};

