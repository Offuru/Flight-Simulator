#include "LightSource.h"


LightSource::LightSource(const std::string& path, bool isMoon) :
	m_sun{ path }
{
	cnt = 0;

	auto givemetime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::tm* time = std::localtime(&givemetime);

	hour = time->tm_hour;
	minute = time->tm_min;
	second = time->tm_sec;
	if (isMoon)
		hour = (hour + 12) % 24;
	day = time->tm_mday;
	month = time->tm_mon + 1;
	year = time->tm_year + 1900;

	this->isMoon = isMoon;
}

void LightSource::Render(Shader& lampShader, const glm::mat4& projection, const glm::mat4& view, glm::mat4& model)
{
	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);
	lampShader.setMat4("model", model);

	if (!isMoon)
		lampShader.setVec3("lightColor", glm::vec3(1.f, 1.f, 0.f));
	else
		lampShader.setVec3("lightColor", glm::vec3(1.f, 1.f, 1.f));

	m_sun.Draw(lampShader);
}

glm::vec3 LightSource::getPos() const
{
	return m_lightPos;
}

glm::vec3 LightSource::updateBySunPosition()
{
	spa_data spa;  //declare the SPA structure
	int result;
	float min, sec;

	//enter required input values into SPA structure

	spa.year = year;
	spa.month = month;
	spa.day = day;
	spa.hour = hour;
	spa.minute = minute;
	spa.second = second;
	spa.timezone = 2;
	spa.delta_ut1 = 0;
	spa.delta_t = 67;
	spa.longitude = -105.1786;
	spa.latitude = 39.742476;
	spa.elevation = 1830.14;
	spa.pressure = 820;
	spa.temperature = 11;
	spa.slope = 30;
	spa.azm_rotation = -10;
	spa.atmos_refract = 0.5667;
	spa.function = SPA_ZA;

	//call the SPA calculate function and pass the SPA structure

	result = spa_calculate(&spa);

	if (result == 0)  //check for SPA errors
	{

		float distance = 999999.0f;
		float x = distance * cos(glm::radians(spa.azimuth)) * sin(glm::radians(spa.zenith));
		float y = distance * sin(glm::radians(spa.azimuth));
		float z = distance * cos(glm::radians(spa.azimuth)) * cos(glm::radians(-spa.zenith));

		m_lightPos = glm::vec3(-x, y, -z);
	}
	else
	{
		printf("SPA Error Code: %d\n", result);
		return glm::vec3(0.f, 0.f, 0.f);
	}

	//std::cout << hour << ":" << minute << ":" << second << '\n';
	return m_lightPos;
}
