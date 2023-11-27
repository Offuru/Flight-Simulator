#pragma once
#include <string>

#include <glew.h>

class Shader
{
public:

	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Use() const;
	
	GLuint GetID() const;

	GLuint modelMatrixLocation;
	GLuint viewMatrixLocation;
	GLuint projMatrixLocation;

private:

	void CheckCompileErrors(GLuint shader, const std::string& type);


	GLuint m_ID;

};

