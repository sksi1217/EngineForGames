#pragma once

#include <glm/glm.hpp>

struct Rigidbody2D
{
	glm::vec2 velocity = glm::vec2(0.0f); // ! скорость
	glm::vec2 acceleration{0.0f, 0.0f};	  // ! ускорение
	float mass = 1.0f;					  // ! масса
	float Restitution = 1;				  // ! Коэффициент упругости
	float Friction = 0.5f;				  // ! Коэффициент трения
	bool isKinematic = false;			  // ! Если true, тело не реагирует на физику

	void AddForce(const glm::vec2 &force)
	{
		acceleration += force / mass;
	}
};

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

struct CircleCollider2D
{
	float radius = 0.5f;
	glm::vec2 offset{0.0f, 0.0f};
	bool isTrigger = false;
};
