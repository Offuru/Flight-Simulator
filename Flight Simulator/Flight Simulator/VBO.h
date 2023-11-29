#pragma once
#include <glew.h>
#include <vector>

class VBO
{
public:
	
	VBO() = default;
	VBO(std::vector<GLfloat> vertices);
	~VBO();

	void Bind();
	void Unbind();

	GLuint getID() const;
private:
	GLuint m_ID;
};

