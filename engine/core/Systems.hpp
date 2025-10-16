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

	class MovementSystem
	{
	public:
		void Update(SpatialPartitioning &spatialPartitioning);
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
