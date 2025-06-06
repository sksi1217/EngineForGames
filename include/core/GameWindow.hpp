#pragma once

#include <GLFW/glfw3.h>
#include <src/utils/Logger.hpp>
#include <include/stb_image.h>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <string>

class GameWindow
{
public:
	// ! Типы коллбэков
	using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
	using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
	using MousePosCallback = std::function<void(double x, double y)>;
	using MouseScrollCallback = std::function<void(double xoffset, double yoffset)>;
	using WindowSizeCallback = std::function<void(int width, int height)>;
	using CharCallback = std::function<void(unsigned int codepoint)>;

	using CallbackID = size_t;

	GameWindow(const std::string &title, int width, int height);
	~GameWindow();

	// ! Основные операции
	void SwapBuffers() const;
	void Close();
	void SetVSync(bool enabled);

	// ! Управление окном
	void SetTitle(const std::string &title);
	void SetSize(int width, int height);
	void SetPosition(int x, int y);
	void Maximize();
	void Minimize();
	void Restore();
	void SetIcon(const std::string &iconPath);

	// ! Состояние окна
	bool ShouldClose() const;
	bool IsVSyncEnabled() const;
	bool IsMaximized() const;
	bool IsMinimized() const;
	bool IsFocused() const;

	// ! Получение параметров
	std::pair<int, int> GetSize() const;
	std::pair<int, int> GetPosition() const;
	std::pair<float, float> GetContentScale() const;
	std::string GetTitle() const;
	inline GLFWwindow *GetWindow() const { return m_Window; }

	// ! Регистрация коллбэков

	// ! --- Клавиатура ---
	CallbackID AddKeyCallback(const KeyCallback &callback);
	bool RemoveKeyCallback(CallbackID id);
	void ClearKeyCallbacks();

	// ! --- Клики мыши ---
	CallbackID AddMouseButtonCallback(const MouseButtonCallback &callback);
	bool RemoveMouseButtonCallback(CallbackID id);
	void ClearMouseButtonCallbacks();

	// ! --- Перемещение курсора ---
	CallbackID AddMousePosCallback(const MousePosCallback &callback);
	bool RemoveMousePosCallback(CallbackID id);
	void ClearMousePosCallbacks();

	// ! --- Прокрутка колеса мыши ---
	CallbackID AddMouseScrollCallback(const MouseScrollCallback &callback);
	bool RemoveMouseScrollCallback(CallbackID id);
	void ClearMouseScrollCallbacks();

	// ! --- Изменение размера окна ---
	CallbackID AddWindowSizeCallback(const WindowSizeCallback &callback);
	bool RemoveWindowSizeCallback(CallbackID id);
	void ClearWindowSizeCallbacks();

	// ! --- Ввод текста ---
	CallbackID AddCharCallback(const CharCallback &callback);
	bool RemoveCharCallback(CallbackID id);
	void ClearCharCallbacks();

private:
	GLFWwindow *m_Window;
	std::string m_Title;
	int m_Width, m_Height;
	bool m_VSync;

	// ! Хранение коллбэков
	std::unordered_map<CallbackID, KeyCallback> m_KeyCallbacks;
	std::unordered_map<CallbackID, MouseButtonCallback> m_MouseButtonCallbacks;
	std::unordered_map<CallbackID, MousePosCallback> m_MousePosCallbacks;
	std::unordered_map<CallbackID, MouseScrollCallback> m_MouseScrollCallbacks;
	std::unordered_map<CallbackID, WindowSizeCallback> m_WindowSizeCallbacks;
	std::unordered_map<CallbackID, CharCallback> m_CharCallbacks;

	CallbackID m_NextCallbackID = 0;

	// ! Статические обработчики GLFW
	static void GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void GLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void GLFWCursorPosCallback(GLFWwindow *window, double x, double y);
	static void GLFWScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
	static void GLFWWindowSizeCallback(GLFWwindow *window, int width, int height);
	static void GLFWCharCallback(GLFWwindow *window, unsigned int codepoint);
};
