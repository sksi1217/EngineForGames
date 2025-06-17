#pragma once

#include <glm/glm.hpp>

struct Rigidbody2D
{
	glm::vec2 velocity = glm::vec2(0.0f); // ! скорость
	glm::vec2 acceleration{0.0f, 0.0f};	  // ! ускорение
	float mass = 1.0f;					  // ! масса
	float Restitution = 1; // ! Коэффициент упругости
    float Friction = 0.5f;  // ! Коэффициент трения
	bool isKinematic = false;			  // ! Если true, тело не реагирует на физику

	void AddForce(const glm::vec2 &force)
	{
		acceleration += force / mass;
	}
};
