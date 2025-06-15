#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

class InputManager
{
public:
	static InputManager &Get()
	{
		static InputManager instance;
		return instance;
	}

	void Update(GLFWwindow *window)
	{
		for (int i = 0; i <= GLFW_KEY_LAST; ++i)
		{
			m_PreviousKeys[i] = m_CurrentKeys[i];
			m_CurrentKeys[i] = glfwGetKey(window, i) == GLFW_PRESS;
		}
	}

	bool IsKeyPressed(int key)
	{
		if (key >= 0 && key <= GLFW_KEY_LAST)
			return m_CurrentKeys[key];
		return false;
	}

	bool WasKeyPressed(int key)
	{
		if (key >= 0 && key <= GLFW_KEY_LAST)
			return m_CurrentKeys[key] && !m_PreviousKeys[key];
		return false;
	}

private:
	std::unordered_map<int, bool> m_CurrentKeys;
	std::unordered_map<int, bool> m_PreviousKeys;
};
