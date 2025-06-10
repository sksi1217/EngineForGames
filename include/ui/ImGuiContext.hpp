#pragma once

#include <iostream>

#include <libs/imgui/imgui.h>
#include <libs/imgui/backends/imgui_impl_glfw.h>
#include <libs/imgui/backends/imgui_impl_opengl3.h>

#include <include/utils/Time.hpp>

// #include <core/GameObject.hpp>

#include <include/core/EventSystem.hpp>
#include <include/core/GameWindow.hpp>

class ImGuiContext
{
public:
	~ImGuiContext();

	static void Init(GLFWwindow *window);
	static void Shutdown();
	static void BeginFrame();
	static void EndFrame();

	static void RenderDebugUI(std::unique_ptr<GameWindow> &window, EventSystem &events, bool *showUI);

	static void ShowSettingsWindow(bool *open, std::unique_ptr<GameWindow> &m_Window);

private:
	static inline char m_InputText[256];
};
