#pragma once

#include <glm/glm.hpp>
#include "graphics/Texture2D.hpp"

struct SpriteComponent
{
	Texture2D *texture;
	glm::vec2 textureOffset{0.0f, 0.0f};
	glm::vec2 textureSize{0.0f, 0.0f};
	glm::vec4 color{1.0f, 0.0f, 0.0f, 1.0f};
	glm::vec2 origin{0.5f, 0.5f};
	bool flipX = false;
	bool flipY = false;
};
