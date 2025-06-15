#pragma once

#include <glm/glm.hpp>

typedef struct RenderParams
{
	glm::vec2 Position{10.0f, 10.0f}; //
	glm::vec2 Scale{50.0f, 50.0f};	  //

	glm::vec2 SpriteOffset{0.0f, 0.0f};		 //
	glm::vec2 SpriteSize{0.0f, 0.0f};		 //
	glm::vec2 Origin{0, 0};					 //
	float Rotation = 0.0f;					 //
	glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f}; //
	bool FlipX = false;						 //
	bool FlipY = false;						 //
} RenderParams;
