#pragma once

#include <extern/imgui/imgui.h>
#include <extern/imgui/backends/imgui_impl_glfw.h>
#include <functional>
#include <engine/core/utils/Logger.hpp>
#include <engine/LightEngine.hpp>
#include <engine/core/graphics/renderer/Renderer.hpp>

#include <GLFW/glfw3.h>

struct EngineState;

class EventSystem
{
private:
	// ! Типы коллбэков
	using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
	using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
	using MousePosCallback = std::function<void(double x, double y)>;
	using MouseScrollCallback = std::function<void(double xoffset, double yoffset)>;
	using WindowSizeCallback = std::function<void(int width, int height)>;
	using CharCallback = std::function<void(unsigned int codepoint)>;

	using CallbackID = size_t;

	struct KeyEvent
	{
		int key;
		int scancode;
		int action;
		int mods;
		float time;
	};

	struct MouseButtonEvent
	{
		int button;
		int action;
		int mods;
		float time;
	};

	struct MousePos
	{
		double x;
		double y;
	};

	struct ScrollEvent
	{
		double x;
		double y;
		float time;
	};

	struct CharInput
	{
		unsigned int character;
		float time;
	};

	struct WindowSize
	{
		int w;
		int h;
	};

	GLFWwindow *m_Window = nullptr;
	EngineState *m_EngineState = nullptr;

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

	// ! Запуск колбэков
	void SetupCallbacks();

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

public:
	KeyEvent m_LastKeyEvent;
	MouseButtonEvent m_LastMouseButtonEvent;
	MousePos m_LastMousePos;
	float m_LastMouseMoveTime = 0.0f;
	ScrollEvent m_LastScrollEvent;
	CharInput m_LastCharInput;
	WindowSize m_LastWindowSize;
	float m_LastWindowResizeTime = 0.0f;

	void Init(GLFWwindow *window, EngineState *engineState);
};
