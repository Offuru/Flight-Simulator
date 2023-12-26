#pragma once
#include <glew/glew.h>
#include <vector>

class VBO
{
public:
	
	VBO();
	~VBO();

	void Initialize(std::vector<GLfloat>& vertices);
	void Bind();
	void Unbind();

	GLuint getID() const;
private:
	GLuint m_ID;
};

