#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include<vector>
#include<iostream>

#include "VAO.h"
#include "VBO.h"
#include "Shader.h"
#include"Camera.h"

class Skybox
{
public:
	Skybox(const std::string&);
	~Skybox();
	void Render(Shader&, Camera*, Plane&);
private:
	void LoadCube();
	GLuint LoadCubeMap(const std::string&);
	VAO vao;
	GLuint texture;
};

