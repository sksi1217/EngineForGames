#pragma once

#include <core/GameObject.hpp>

#include <include/core/EventSystem.hpp>
#include <iostream>
#include <include/core/GameWindow.hpp>

#include <libs/imgui/imgui.h>
#include <libs/imgui/backends/imgui_impl_glfw.h>
#include <libs/imgui/backends/imgui_impl_opengl3.h>
#include <src/utils/Time.hpp>

#include <GLFW/glfw3.h>

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

private:
	static inline char m_InputText[256];
};
