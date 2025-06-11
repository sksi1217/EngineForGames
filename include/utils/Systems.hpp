#pragma once
#include <core/ECS.hpp>
#include <include/graphics/Renderer.hpp>

#include <include/utils/Time.hpp>

#include <include/core/SpatialPartitioning.hpp>

#include <include/components/Components.hpp>

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
