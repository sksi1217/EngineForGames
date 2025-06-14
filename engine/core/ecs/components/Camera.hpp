#pragma once
#include <glm/glm.hpp>

struct Camera
{
	float zoom = 1.0f;
	// glm::vec2 offset = {0.0f, 0.0f};
	// float rotation = 0;
	bool isMain = false;
};
