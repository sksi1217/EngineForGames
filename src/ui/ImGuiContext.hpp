#pragma once

#include <iostream>

#include <libs/imgui/imgui.h>
#include <libs/imgui/backends/imgui_impl_glfw.h>
#include <libs/imgui/backends/imgui_impl_opengl3.h>

#include <src/utils/Time.hpp>

#include <core/GameObject.hpp>

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

	static void RenderDebugUI(GameWindow *window, EventSystem &events, bool *showUI, GameObject *gameObject = nullptr);

	static void RenderObjectUI(GameObject *gameObject);

	static void ShowSettingsWindow(bool *open, GameWindow *m_Window);

private:
	static inline char m_InputText[256];
};
