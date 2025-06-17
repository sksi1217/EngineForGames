#pragma once
#include <engine/engineapi.hpp>

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

class CollisionSystem
{
public:
	void Update(SpatialPartitioning &spatialPartitioning);

private:
	void ResolveCollision(entt::registry &registry, entt::entity a, entt::entity b);

	bool CheckAABB(const BoxCollider2D &a, const Transform &ta, const BoxCollider2D &b, const Transform &tb);
	void HandleCollision(entt::entity a, entt::entity b);
};

class ScriptSystem
{
public:
	void Update();
	void FixedUpdate();
};
