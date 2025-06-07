#include <include/core/EventSystem.hpp>

void EventSystem::Init(GLFWwindow *window, EngineState *engineState)
{
	if (!window)
	{
		utils::Logger::error("GLFW window is null!");
		throw std::runtime_error("GLFW window is null");
	}

	m_Window = window;
	m_EngineState = engineState;

	// ! Сохраняем указатель на наш объект Window
	glfwSetWindowUserPointer(m_Window, this);
	utils::Logger::info("GLFW window created successfully");

	SetupCallbacks();
}

// ! Статические обработчики GLFW
#pragma region Статические обработчики GLFW

void EventSystem::GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	EventSystem *win = static_cast<EventSystem *>(glfwGetWindowUserPointer(window));
	if (!win)
		return;

	for (const auto &[id, callback] : win->m_KeyCallbacks)
	{
		if (callback)
		{
			try
			{
				callback(key, scancode, action, mods);
			}
			catch (const std::exception &e)
			{
				std::cerr << "Key callback error: " << e.what() << std::endl;
			}
		}
	}
}

void EventSystem::GLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	EventSystem *win = static_cast<EventSystem *>(glfwGetWindowUserPointer(window));
	if (!win)
		return;

	for (const auto &[id, callback] : win->m_MouseButtonCallbacks)
	{
		if (callback)
		{
			try
			{
				callback(button, action, mods);
			}
			catch (const std::exception &e)
			{
				std::cerr << "Mouse button callback error: " << e.what() << std::endl;
			}
		}
	}
}

void EventSystem::GLFWCursorPosCallback(GLFWwindow *window, double x, double y)
{
	EventSystem *win = static_cast<EventSystem *>(glfwGetWindowUserPointer(window));
	if (!win)
		return;

	for (const auto &[id, callback] : win->m_MousePosCallbacks)
	{
		if (callback)
		{
			try
			{
				callback(x, y);
			}
			catch (const std::exception &e)
			{
				std::cerr << "Mouse position callback error: " << e.what() << std::endl;
			}
		}
	}
}

void EventSystem::GLFWScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	EventSystem *win = static_cast<EventSystem *>(glfwGetWindowUserPointer(window));
	if (!win)
		return;

	for (const auto &[id, callback] : win->m_MouseScrollCallbacks)
	{
		if (callback)
		{
			try
			{
				callback(xoffset, yoffset);
			}
			catch (const std::exception &e)
			{
				std::cerr << "Mouse scroll callback error: " << e.what() << std::endl;
			}
		}
	}
}

void EventSystem::GLFWWindowSizeCallback(GLFWwindow *window, int width, int height)
{
	EventSystem *win = static_cast<EventSystem *>(glfwGetWindowUserPointer(window));
	if (!win)
		return;

	for (const auto &[id, callback] : win->m_WindowSizeCallbacks)
	{
		if (callback)
		{
			try
			{
				callback(width, height);
			}
			catch (const std::exception &e)
			{
				std::cerr << "Window size callback error: " << e.what() << std::endl;
			}
		}
	}
}

void EventSystem::GLFWCharCallback(GLFWwindow *window, unsigned int codepoint)
{
	EventSystem *win = static_cast<EventSystem *>(glfwGetWindowUserPointer(window));
	if (!win)
		return;

	for (const auto &[id, callback] : win->m_CharCallbacks)
	{
		if (callback)
		{
			try
			{
				callback(codepoint);
			}
			catch (const std::exception &e)
			{
				std::cerr << "Char callback error: " << e.what() << std::endl;
			}
		}
	}
}

#pragma endregion

// ! Регистрация callback
#pragma region Регистрация callback

// --- Клавиатура ---
EventSystem::CallbackID EventSystem::AddKeyCallback(const KeyCallback &callback)
{
	utils::Logger::debug("Registering new key callback");

	if (m_KeyCallbacks.empty())
	{
		glfwSetKeyCallback(m_Window, GLFWKeyCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_KeyCallbacks[id] = callback;
	return id;
}

bool EventSystem::RemoveKeyCallback(CallbackID id)
{
	auto it = m_KeyCallbacks.find(id);
	if (it != m_KeyCallbacks.end())
	{
		utils::Logger::debug("Removing key callback with ID: " + std::to_string(id));
		m_KeyCallbacks.erase(it);

		if (m_KeyCallbacks.empty())
		{
			glfwSetKeyCallback(m_Window, nullptr);
		}
		return true;
	}
	utils::Logger::warning("Failed to remove key callback with ID: " + std::to_string(id));

	return false;
}

void EventSystem::ClearKeyCallbacks()
{
	m_KeyCallbacks.clear();
	glfwSetKeyCallback(m_Window, nullptr);
}

// --- Клики мыши ---
EventSystem::CallbackID EventSystem::AddMouseButtonCallback(const MouseButtonCallback &callback)
{
	if (m_MouseButtonCallbacks.empty())
	{
		glfwSetMouseButtonCallback(m_Window, GLFWMouseButtonCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_MouseButtonCallbacks[id] = callback;
	return id;
}

bool EventSystem::RemoveMouseButtonCallback(CallbackID id)
{
	auto it = m_MouseButtonCallbacks.find(id);
	if (it != m_MouseButtonCallbacks.end())
	{
		m_MouseButtonCallbacks.erase(it);

		if (m_MouseButtonCallbacks.empty())
		{
			glfwSetMouseButtonCallback(m_Window, nullptr);
		}
		return true;
	}
	return false;
}

void EventSystem::ClearMouseButtonCallbacks()
{
	m_MouseButtonCallbacks.clear();
	glfwSetMouseButtonCallback(m_Window, nullptr);
}

// --- Перемещение курсора ---
EventSystem::CallbackID EventSystem::AddMousePosCallback(const MousePosCallback &callback)
{
	if (m_MousePosCallbacks.empty())
	{
		glfwSetCursorPosCallback(m_Window, GLFWCursorPosCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_MousePosCallbacks[id] = callback;
	return id;
}

bool EventSystem::RemoveMousePosCallback(CallbackID id)
{
	auto it = m_MousePosCallbacks.find(id);
	if (it != m_MousePosCallbacks.end())
	{
		m_MousePosCallbacks.erase(it);

		if (m_MousePosCallbacks.empty())
		{
			glfwSetCursorPosCallback(m_Window, nullptr);
		}
		return true;
	}
	return false;
}

void EventSystem::ClearMousePosCallbacks()
{
	m_MousePosCallbacks.clear();
	glfwSetCursorPosCallback(m_Window, nullptr);
}

// --- Прокрутка колеса мыши ---
EventSystem::CallbackID EventSystem::AddMouseScrollCallback(const MouseScrollCallback &callback)
{
	if (m_MouseScrollCallbacks.empty())
	{
		glfwSetScrollCallback(m_Window, GLFWScrollCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_MouseScrollCallbacks[id] = callback;
	return id;
}

bool EventSystem::RemoveMouseScrollCallback(CallbackID id)
{
	auto it = m_MouseScrollCallbacks.find(id);
	if (it != m_MouseScrollCallbacks.end())
	{
		m_MouseScrollCallbacks.erase(it);

		if (m_MouseScrollCallbacks.empty())
		{
			glfwSetScrollCallback(m_Window, nullptr);
		}
		return true;
	}
	return false;
}

void EventSystem::ClearMouseScrollCallbacks()
{
	m_MouseScrollCallbacks.clear();
	glfwSetScrollCallback(m_Window, nullptr);
}

// --- Изменение размера окна ---
EventSystem::CallbackID EventSystem::AddWindowSizeCallback(const WindowSizeCallback &callback)
{
	if (m_WindowSizeCallbacks.empty())
	{
		glfwSetWindowSizeCallback(m_Window, GLFWWindowSizeCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_WindowSizeCallbacks[id] = callback;
	return id;
}

bool EventSystem::RemoveWindowSizeCallback(CallbackID id)
{
	auto it = m_WindowSizeCallbacks.find(id);
	if (it != m_WindowSizeCallbacks.end())
	{
		m_WindowSizeCallbacks.erase(it);

		if (m_WindowSizeCallbacks.empty())
		{
			glfwSetWindowSizeCallback(m_Window, nullptr);
		}
		return true;
	}
	return false;
}

void EventSystem::ClearWindowSizeCallbacks()
{
	m_WindowSizeCallbacks.clear();
	glfwSetWindowSizeCallback(m_Window, nullptr);
}

// Регистрация callback'а для ввода символов
EventSystem::CallbackID EventSystem::AddCharCallback(const CharCallback &callback)
{
	utils::Logger::debug("Registering new char callback");

	if (m_CharCallbacks.empty())
	{
		glfwSetCharCallback(m_Window, GLFWCharCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_CharCallbacks[id] = callback;
	return id;
}

// Удаление callback'а для ввода символов
bool EventSystem::RemoveCharCallback(CallbackID id)
{
	auto it = m_CharCallbacks.find(id);
	if (it != m_CharCallbacks.end())
	{
		utils::Logger::debug("Removing char callback with ID: " + std::to_string(id));
		m_CharCallbacks.erase(it);

		if (m_CharCallbacks.empty())
		{
			glfwSetCharCallback(m_Window, nullptr);
		}
		return true;
	}
	utils::Logger::warning("Failed to remove char callback with ID: " + std::to_string(id));
	return false;
}

// Очистка всех callback'ов для ввода символов
void EventSystem::ClearCharCallbacks()
{
	m_CharCallbacks.clear();
	glfwSetCharCallback(m_Window, nullptr);
}

#pragma endregion

#pragma region CallBacks
void EventSystem::SetupCallbacks()
{
	// ! Клавиатура
	AddKeyCallback(
		[this](int key, int scancode, int action, int mods)
		{
			// ! Обновляем состояние клавиш
			if (key >= 0 && key < 512)
			{
				m_EngineState->keys[key] = (action != GLFW_RELEASE);
			}

			// ! Сохраняем информацию о событии
			m_LastKeyEvent = {key, scancode, action, mods, static_cast<float>(glfwGetTime())};

			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				m_EngineState->isRunning = false;
			}

			if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			{
				m_EngineState->showDebugUI = !m_EngineState->showDebugUI;
			}

			if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
			{
				m_EngineState->showSettingsWindow = !m_EngineState->showSettingsWindow;
			}
		});

	// Клик мыши
	AddMouseButtonCallback(
		[this](int button, int action, int mods)
		{
			if (button >= 0 && button < 8)
			{
				m_EngineState->mouseButtons[button] = (action != GLFW_RELEASE);
			}

			// ! Сохраняем информацию о событии
			m_LastMouseButtonEvent = {
				button,
				action,
				mods,
				static_cast<float>(glfwGetTime())};

			// ! Передаем события мыши в ImGui
			ImGuiIO &io = ImGui::GetIO();
			if (button >= 0 && button < ImGuiMouseButton_COUNT)
				io.MouseDown[button] = (action == GLFW_PRESS);
		});

	// ! Перемещение мыши
	AddMousePosCallback(
		[this](double x, double y)
		{
			m_EngineState->mouseX = x;
			m_EngineState->mouseY = y;

			// ! Сохраняем позицию мыши
			m_LastMousePos = {x, y};
			m_LastMouseMoveTime = static_cast<float>(glfwGetTime());

			// ! Передаем позицию мыши в ImGui
			ImGuiIO &io = ImGui::GetIO();
			io.MousePos = ImVec2((float)x, (float)y);
		});

	// ! Прокрутка
	AddMouseScrollCallback(
		[this](double x, double y)
		{
			// ! Сохраняем информацию о прокрутке
			m_LastScrollEvent = {
				x,
				y,
				static_cast<float>(glfwGetTime())};

			// ! Передаем прокрутку в ImGui
			ImGuiIO &io = ImGui::GetIO();
			io.MouseWheelH += (float)x;
			io.MouseWheel += (float)y;
		});

	// ! Ввод текста
	AddCharCallback(
		[this](unsigned int c)
		{
			// ! Сохраняем введённый символ
			m_LastCharInput = {c, static_cast<float>(glfwGetTime())};
		});

	// ! Изменение размера окна
	AddWindowSizeCallback(
		[this](int w, int h)
		{
			m_LastWindowSize = {w, h};
			m_LastWindowResizeTime = static_cast<float>(glfwGetTime());

			// ! Обновляем viewport рендерера
			Renderer::Get().SetViewportSize(w, h);
		});
}
#pragma endregion
