#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string>

#include <glew/glew.h>
#include <glm/GLM.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfw/glfw3.h>

#include "Shader.h"
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"

#include <chrono>
#include "spa.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Object.h"

class LightSource
{
public:
	LightSource(const std::string& path, bool isMoon = false);
	~LightSource() = default;

	void Render(Shader& lampShader, const glm::mat4& projection, const glm::mat4& view, glm::mat4& model);

	glm::vec3 getPos() const;

	glm::vec3 updateBySunPosition();


	void increment()
	{
		second += 100;

		minute += (int)second / 60;
		hour += minute / 60;
		hour %= 24;
		minute %= 60;
		if (second >= 60.f)
			second = 0.f;
	}
private:

	glm::vec3 m_lightPos{ 0.f, 0.f, 0.f };
	std::vector<GLuint> m_indices;
	std::vector<GLfloat> m_vertices;

	Model m_sun;

	int hour;
	int minute;
	float second;

	int day;
	int month;
	int year;

	int cnt;

	bool isMoon;
};

