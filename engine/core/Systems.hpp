#pragma once
#include <engine/LightEngine.hpp>

#include <engine/core/ecs/components/CoreComponents.hpp>
#include <engine/core/ecs/components/PhysicsComponents.hpp>

#include <engine/core/physics/CollisionDetection.hpp>
#include <engine/core/physics/CollisionResolution.hpp>
#include <engine/core/physics/CollisionShapes.hpp>
#include <engine/core/physics/SpatialHashGrid.hpp>

#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Destruction.hpp>
#include <extern/entt/entt.hpp>

#include <engine/core/scene/Object.hpp>

#include <engine/core/graphics/renderer/Renderer.hpp>

#include <engine/core/ecs/ECS.hpp>

namespace le
{

	class DebugDrawSystem
	{
	public:
		// Глобальные ресурсы для отладочной отрисовки (инициализируются один раз)
		GLuint g_debugVao = 0;
		GLuint g_debugVbo = 0;
		bool g_initialized = false;

		void InitDebugDraw()
		{
			if (g_initialized)
				return;

			glGenVertexArrays(1, &g_debugVao);
			glGenBuffers(1, &g_debugVbo);

			glBindVertexArray(g_debugVao);
			glBindBuffer(GL_ARRAY_BUFFER, g_debugVbo);
			// Будем передавать данные каждый кадр — динамический буфер
			glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

			// layout (location = 0) in vec2 aPos;
			// layout (location = 1) in vec3 aColor;
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 2));
			glEnableVertexAttribArray(1);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			g_initialized = true;
		}

		// Обновляет отладочную информацию: собирает коллайдеры и отправляет их в DebugRenderer
		void Update(entt::registry &registry);
	};

	class CameraSystem
	{
	public:
		void Update(entt::registry &registry, Renderer &renderer);
	};

	class RenderSystem
	{
	public:
		void Update();
	};

	class ScriptSystem
	{
	public:
		void Update();
		void FixedUpdate();
	};

	class DestroySystem
	{
	public:
		static void Update();
	};

	class PhysicsSystem
	{
	public:
		PhysicsSystem(float worldWidth = 1000.0f, float worldHeight = 1000.0f, float cellSize = 100.0f);

		void Update(entt::registry &registry, float dt);

		void SetWorldBounds(float width, float height)
		{
			m_worldWidth = width;
			m_worldHeight = height;
		}

	private:
		void IntegratePositions(entt::registry &registry, float dt);
		void UpdateBroadPhase(entt::registry &registry);
		void ResolveCollisions(entt::registry &registry, float dt);
		void ResolveWorldBounds(entt::registry &registry);

		float m_worldWidth;
		float m_worldHeight;
		SpatialHashGrid m_grid;
	};

}
