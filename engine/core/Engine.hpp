#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <extern/imgui/imgui.h>
#include <extern/imgui/backends/imgui_impl_glfw.h>
#include <extern/imgui/backends/imgui_impl_opengl3.h>

// --
#include <engine/core/EngineState.hpp>
#include <engine/core/events/EventSystem.hpp>
#include <engine/core/window/GameWindow.hpp>
#include <engine/core/scene/GameObject.hpp>

#include <engine/core/SpatialPartitioning.hpp>

#include <engine/core/utils/Logger.hpp>
#include <engine/core/utils/Time.hpp>
#include <engine/core/graphics/textures/TextureLoader.hpp>

#include <engine/core/graphics/renderer/GLContext.hpp>

#include <engine/core/Systems.hpp>

#include <engine/core/ui/ImGuiContext.hpp>

#include <engine/core/ui/Settings.hpp>

class Engine
{
public:
	Engine();
	~Engine();

	void Run();

private:
	std::unique_ptr<GameWindow> m_Window;
	GLContext glContext;
	EngineState m_State;
	EventSystem events;

	SpatialPartitioning *spatialPartitioning;

	// Системы
	RenderSystem renderSystem;
	MovementSystem movementSystem;
	CollisionSystem collisionSystem;
	ScriptSystem scriptSystem;

	void Initialize();
	void Update();
	void Draw();
};
