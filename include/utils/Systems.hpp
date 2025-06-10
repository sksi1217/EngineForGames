#pragma once
#include <core/ECS.hpp>
#include <include/graphics/Renderer.hpp>
#include <include/core/components/TransformComponent.hpp>
#include <include/core/components/SpriteComponent.hpp>
#include <include/core/SpatialPartitioning.hpp>

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
