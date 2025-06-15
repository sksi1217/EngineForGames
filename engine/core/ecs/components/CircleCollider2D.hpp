#pragma once

#include <glm/glm.hpp>

struct CircleCollider2D
{
	float radius = 0.5f;
	glm::vec2 offset{0.0f, 0.0f};
	bool isTrigger = false;
};
