#pragma once

#include <glew.h>
#include <vector>

class EBO
{
public:

	EBO();
	~EBO();

	void Initialize(std::vector<GLuint>& indices);
	void Bind();
	void Unbind();

	
private:
	GLuint m_ID;
	
};

