#pragma once
#include <string>

#include <glew.h>
#include <glm/fwd.hpp>

class Shader
{
public:

	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void use() const;
	
	GLuint getID() const;

	GLuint modelMatrixLocation;
	GLuint viewMatrixLocation;
	GLuint projMatrixLocation;

	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:

	void checkCompileErrors(GLuint shader, const std::string& type);

	GLuint m_ID;

};

