#pragma once

#include <glm/glm.hpp>

struct BoxCollider2D
{
	glm::vec2 size{1.0f, 1.0f};	  // ! Ширина и высота
	glm::vec2 offset{0.0f, 0.0f}; // ! Смещение относительно позиции объекта
	bool isTrigger = false;		  // ! Если true, не вызывает физического ответа

	// Для оптимизации можно кэшировать AABB
	mutable glm::vec2 cachedMin;
	mutable glm::vec2 cachedMax;
	mutable bool cacheValid = false;
};
