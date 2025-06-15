#pragma once

#include <engine/engineapi.hpp>

#include <extern/imgui/imgui.h>
#include <extern/imgui/backends/imgui_impl_glfw.h>
#include <extern/imgui/backends/imgui_impl_opengl3.h>
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
