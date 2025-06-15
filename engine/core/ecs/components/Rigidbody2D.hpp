#pragma once

#include <glm/glm.hpp>

struct Rigidbody2D
{
	glm::vec2 velocity{0.0f, 0.0f};		// ! скорость
	glm::vec2 acceleration{0.0f, 0.0f}; // ! ускорение
	float mass = 1.0f;					// ! масса
	float drag = 0.1f;					// !
	bool isKinematic = false;			// ! Если true, тело не реагирует на физику

	void AddForce(const glm::vec2 &force)
	{
		acceleration += force / mass;
	}
};
