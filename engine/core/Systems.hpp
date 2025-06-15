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
};

class ScriptSystem
{
public:
	void Update();
	void FixedUpdate();
};
