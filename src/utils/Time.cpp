#include "Time.hpp"
#include <GLFW/glfw3.h>

float utils::Time::m_deltaTime = 0.0f;
float utils::Time::m_lastFrame = 0.0f;
float utils::Time::m_fps = 0.0f;
int utils::Time::m_frameCount = 0;
float utils::Time::m_fpsLastTime = 0.0f;

void utils::Time::Update()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;

	// FPS calculation
	m_frameCount++;
	if (currentFrame - m_fpsLastTime >= 1.0f)
	{
		m_fps = static_cast<float>(m_frameCount);
		m_frameCount = 0;
		m_fpsLastTime = currentFrame;
	}
}

float utils::Time::DeltaTime()
{
	return m_deltaTime;
}

float utils::Time::FPS()
{
	return m_fps;
}
