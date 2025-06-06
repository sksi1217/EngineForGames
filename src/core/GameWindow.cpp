#include <include/core/GameWindow.hpp>

// ! Начальные методы
#pragma region Начальные методы

GameWindow::GameWindow(const std::string &title, int width, int height) : m_Title(title), m_Width(width), m_Height(height)
{
	utils::Logger::info("Initializing GLFW...");

	if (!glfwInit())
	{
		utils::Logger::error("Failed to initialize GLFW");
		throw std::runtime_error("Failed to initialize GLFW");
	}

	// ! Настройка OpenGL контекста
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);				   // ? Указывает основную версию OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);				   // ? Указывает побочную версию OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ? Определяет, какой профиль OpenGL  использовать

	// ! Создание окна
	utils::Logger::info("Creating GLFW window...");
	m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	// ! Проверка
	if (!m_Window)
	{
		utils::Logger::error("Failed to create GLFW window");
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(m_VSync ? 1 : 0);

	// ! Сохраняем указатель на наш объект Window
	glfwSetWindowUserPointer(m_Window, this);
	utils::Logger::info("GLFW window created successfully");
}

GameWindow::~GameWindow()
{
	utils::Logger::info("Destroying GLFW window...");
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
#pragma endregion

// ! Основные операции
#pragma region Основные операции

void GameWindow::SwapBuffers() const
{
	if (!m_Window)
	{
		utils::Logger::error("Invalid GLFW window handle...");
		throw std::runtime_error("Invalid GLFW window handle");
	}

	// ! Меняем буферы (двойная буферизация)
	glfwSwapBuffers(m_Window);
}

void GameWindow::Close()
{
	utils::Logger::info("Closing window...");
	glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

void GameWindow::SetVSync(bool enabled)
{
	m_VSync = enabled;
	utils::Logger::info("Setting VSync: " + std::string(enabled ? "ON" : "OFF"));
	glfwSwapInterval(enabled ? 1 : 0);
}

#pragma endregion

// ! Управление окном
#pragma region Управление окном

void GameWindow::SetTitle(const std::string &title)
{
	m_Title = title;
	utils::Logger::info("Setting window title to: " + title);
	glfwSetWindowTitle(m_Window, title.c_str());
}

void GameWindow::SetSize(int width, int height)
{
	utils::Logger::info("Setting window size to " + std::to_string(width) + "x" + std::to_string(height));
	m_Width = width;
	m_Height = height;
	glfwSetWindowSize(m_Window, width, height);
}

void GameWindow::SetPosition(int x, int y)
{
	utils::Logger::info("Setting window position to (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	glfwSetWindowPos(m_Window, x, y);
}

void GameWindow::Maximize()
{
	utils::Logger::info("Maximizing window");
	glfwMaximizeWindow(m_Window);
}

void GameWindow::Minimize()
{
	utils::Logger::info("Minimizing window");
	glfwIconifyWindow(m_Window);
}

void GameWindow::Restore()
{
	utils::Logger::info("Restoring window");
	glfwRestoreWindow(m_Window);
}

void GameWindow::SetIcon(const std::string &iconPath)
{
	GLFWimage icon;
	icon.pixels = stbi_load(iconPath.c_str(), &icon.width, &icon.height, nullptr, 4);
	if (icon.pixels)
	{
		utils::Logger::info("Setting window icon from file: " + iconPath);
		glfwSetWindowIcon(m_Window, 1, &icon);
		stbi_image_free(icon.pixels);
	}
	else
	{
		utils::Logger::warning("Failed to load window icon from: " + iconPath);
	}
}

#pragma endregion

// ! Состояние окна
#pragma region Состояние окна

bool GameWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

bool GameWindow::IsVSyncEnabled() const
{
	return m_VSync;
}

bool GameWindow::IsMaximized() const
{
	return glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
}

bool GameWindow::IsMinimized() const
{
	return glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED);
}

bool GameWindow::IsFocused() const
{
	return glfwGetWindowAttrib(m_Window, GLFW_FOCUSED);
}

#pragma endregion

// ! Получение параметров
#pragma region Получение параметров

std::pair<int, int> GameWindow::GetSize() const
{
	int width, height;
	glfwGetWindowSize(m_Window, &width, &height);
	return {width, height};
}

std::pair<int, int> GameWindow::GetPosition() const
{
	int x, y;
	glfwGetWindowPos(m_Window, &x, &y);
	return {x, y};
}

std::pair<float, float> GameWindow::GetContentScale() const
{
	float xscale, yscale;
	glfwGetWindowContentScale(m_Window, &xscale, &yscale);
	return {xscale, yscale};
}

std::string GameWindow::GetTitle() const
{
	return m_Title;
}

#pragma endregion

// ! Статические обработчики GLFW
#pragma region Статические обработчики GLFW

void GameWindow::GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	GameWindow *win = static_cast<GameWindow *>(glfwGetWindowUserPointer(window));
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

void GameWindow::GLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	GameWindow *win = static_cast<GameWindow *>(glfwGetWindowUserPointer(window));
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

void GameWindow::GLFWCursorPosCallback(GLFWwindow *window, double x, double y)
{
	GameWindow *win = static_cast<GameWindow *>(glfwGetWindowUserPointer(window));
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

void GameWindow::GLFWScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	GameWindow *win = static_cast<GameWindow *>(glfwGetWindowUserPointer(window));
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

void GameWindow::GLFWWindowSizeCallback(GLFWwindow *window, int width, int height)
{
	GameWindow *win = static_cast<GameWindow *>(glfwGetWindowUserPointer(window));
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

void GameWindow::GLFWCharCallback(GLFWwindow *window, unsigned int codepoint)
{
	GameWindow *win = static_cast<GameWindow *>(glfwGetWindowUserPointer(window));
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
GameWindow::CallbackID GameWindow::AddKeyCallback(const KeyCallback &callback)
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

bool GameWindow::RemoveKeyCallback(CallbackID id)
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

void GameWindow::ClearKeyCallbacks()
{
	m_KeyCallbacks.clear();
	glfwSetKeyCallback(m_Window, nullptr);
}

// --- Клики мыши ---
GameWindow::CallbackID GameWindow::AddMouseButtonCallback(const MouseButtonCallback &callback)
{
	if (m_MouseButtonCallbacks.empty())
	{
		glfwSetMouseButtonCallback(m_Window, GLFWMouseButtonCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_MouseButtonCallbacks[id] = callback;
	return id;
}

bool GameWindow::RemoveMouseButtonCallback(CallbackID id)
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

void GameWindow::ClearMouseButtonCallbacks()
{
	m_MouseButtonCallbacks.clear();
	glfwSetMouseButtonCallback(m_Window, nullptr);
}

// --- Перемещение курсора ---
GameWindow::CallbackID GameWindow::AddMousePosCallback(const MousePosCallback &callback)
{
	if (m_MousePosCallbacks.empty())
	{
		glfwSetCursorPosCallback(m_Window, GLFWCursorPosCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_MousePosCallbacks[id] = callback;
	return id;
}

bool GameWindow::RemoveMousePosCallback(CallbackID id)
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

void GameWindow::ClearMousePosCallbacks()
{
	m_MousePosCallbacks.clear();
	glfwSetCursorPosCallback(m_Window, nullptr);
}

// --- Прокрутка колеса мыши ---
GameWindow::CallbackID GameWindow::AddMouseScrollCallback(const MouseScrollCallback &callback)
{
	if (m_MouseScrollCallbacks.empty())
	{
		glfwSetScrollCallback(m_Window, GLFWScrollCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_MouseScrollCallbacks[id] = callback;
	return id;
}

bool GameWindow::RemoveMouseScrollCallback(CallbackID id)
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

void GameWindow::ClearMouseScrollCallbacks()
{
	m_MouseScrollCallbacks.clear();
	glfwSetScrollCallback(m_Window, nullptr);
}

// --- Изменение размера окна ---
GameWindow::CallbackID GameWindow::AddWindowSizeCallback(const WindowSizeCallback &callback)
{
	if (m_WindowSizeCallbacks.empty())
	{
		glfwSetWindowSizeCallback(m_Window, GLFWWindowSizeCallback);
	}

	CallbackID id = m_NextCallbackID++;
	m_WindowSizeCallbacks[id] = callback;
	return id;
}

bool GameWindow::RemoveWindowSizeCallback(CallbackID id)
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

void GameWindow::ClearWindowSizeCallbacks()
{
	m_WindowSizeCallbacks.clear();
	glfwSetWindowSizeCallback(m_Window, nullptr);
}

// Регистрация callback'а для ввода символов
GameWindow::CallbackID GameWindow::AddCharCallback(const CharCallback &callback)
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
bool GameWindow::RemoveCharCallback(CallbackID id)
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
void GameWindow::ClearCharCallbacks()
{
	m_CharCallbacks.clear();
	glfwSetCharCallback(m_Window, nullptr);
}

#pragma endregion
