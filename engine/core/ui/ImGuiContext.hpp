#pragma once

#include <iostream>

#include <extern/imgui/imgui.h>
#include <extern/imgui/backends/imgui_impl_glfw.h>
#include <extern/imgui/backends/imgui_impl_opengl3.h>

#include <engine/core/utils/Time.hpp>

#include <engine/core/scene/GameObject.hpp>

#include <engine/core/events/EventSystem.hpp>
#include <engine/core/window/GameWindow.hpp>

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

	static void RenderObjectUI(Transform &transform);

private:
	static inline char m_InputText[256];
};
