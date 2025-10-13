#pragma once

// #include <engine/engineapi.hpp>

#include <random>
#include <thread>

// ** imgui
#include <extern/imgui/imgui.h>
#include <extern/imgui/backends/imgui_impl_glfw.h>
#include <extern/imgui/backends/imgui_impl_opengl3.h>
// ** ---

#include <engine/core/Systems.hpp>
#include <engine/core/ui/ImGuiContext.hpp>
#include <engine/core/graphics/renderer/GLContext.hpp>

#include <engine/core/ecs/components/PhysicsComponents.hpp>

#include <engine/core/ecs/ECS.hpp>

#include <engine/core/events/EventSystem.hpp>

#include <engine/core/SpatialPartitioning.hpp>

#include <engine/core/window/GameWindow.hpp>

#include <engine/core/graphics/textures/TextureLoader.hpp>

#include <engine/core/events/InputManager.hpp>

// #include <engine/core/ui/Settings.hpp>

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
