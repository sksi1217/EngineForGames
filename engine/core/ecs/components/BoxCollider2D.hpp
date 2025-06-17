#pragma once

#include <glm/glm.hpp>

struct BoxCollider2D
{
	glm::vec2 Scale{50.0f, 50.0f}; // ! Ширина и высота
	glm::vec2 offset{0.0f, 0.0f};  // ! Смещение относительно позиции объекта
	bool isTrigger = false;		   // ! Если true, не вызывает физического ответа

	glm::vec2 GetMin(const Transform &transform) const
	{
		return transform.Position + offset - Scale * 0.5f;
	}

	glm::vec2 GetMax(const Transform &transform) const
	{
		return transform.Position + offset + Scale * 0.5f;
	}

	// Для оптимизации можно кэшировать AABB
	mutable glm::vec2 cachedMin;
	mutable glm::vec2 cachedMax;
	mutable bool cacheValid = false;
};
