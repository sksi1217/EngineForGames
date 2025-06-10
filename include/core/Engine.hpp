#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <libs/imgui/imgui.h>
#include <libs/imgui/backends/imgui_impl_glfw.h>
#include <libs/imgui/backends/imgui_impl_opengl3.h>

// --
#include <include/core/EngineState.hpp>
#include <include/core/EventSystem.hpp>
#include <include/core/GameWindow.hpp>
#include <include/core/GameObject.hpp>

#include <include/core/SpatialPartitioning.hpp>

#include <include/utils/Logger.hpp>
#include <include/utils/Time.hpp>
#include <include/utils/TextureLoader.hpp>

#include <include/graphics/GLContext.hpp>

#include <include/ui/ImGuiContext.hpp>

#include <include/utils/Systems.hpp>

class Engine
{
public:
	Engine();
	~Engine();

	void Run();

	// std::vector<std::shared_ptr<entt::entity>> gameObj;
	std::vector<entt::entity> gameEntities;

private:
	std::unique_ptr<GameWindow> m_Window;
	GLContext glContext;
	EngineState m_State;
	EventSystem events;

	// Системы
	RenderSystem renderSystem;
	MovementSystem movementSystem;

	SpatialPartitioning *spatialPartitioning;

	void Initialize();
	void Update();
	void Draw();
};
