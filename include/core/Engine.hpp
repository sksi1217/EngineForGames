#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <libs/imgui/imgui.h>
#include <libs/imgui/backends/imgui_impl_glfw.h>
#include <libs/imgui/backends/imgui_impl_opengl3.h>

// --
#include <include/core/GameWindow.hpp>
#include <include/core/EventSystem.hpp>
#include <include/core/GameObject.hpp>

#include <src/utils/Logger.hpp>
#include <src/utils/Time.hpp>
#include <src/utils/TextureLoader.hpp>
#include <src/utils/ShaderLoader.hpp>

#include <src/graphics/GLContext.hpp>

#include <src/ui/ImGuiContext.hpp>

class Engine
{
public:
	Engine();
	~Engine();

	void Run();

	std::vector<std::shared_ptr<GameObject>> gameObj;

private:
	EventSystem events;
	GameWindow *m_Window;
	GLContext glContext;

	bool m_ShowDebugUI = true;
	bool m_Running = true;

	void Initialize();
	void Update();
	void Draw();

	void SetupCallbacks();
};
