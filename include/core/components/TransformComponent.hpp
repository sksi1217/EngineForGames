#pragma once
#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec2 position{0.0f, 0.0f};
	glm::vec2 scale{50.0f, 50.0f};
	glm::vec2 origin{0.5f, 0.5f};
	float rotation = 0.0f;
};
