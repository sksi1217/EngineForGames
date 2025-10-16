// engine/core/physics/CollisionDetection.hpp
#pragma once
#include "CollisionShapes.hpp"

namespace le
{

	struct SweptResult
	{
		bool hit = false;
		float time = 1.0f;		  // время входа в коллизию (0..1)
		glm::vec2 normal{0, 0};	  // нормаль столкновения
		float penetration = 0.0f; // проникновение (для fallback)
	};

	SweptResult sweptAABB(const Aabb &a, const glm::vec2 &velA, const Aabb &b, float dt);

	Contact collide(const Aabb &a, const Aabb &b);
	Contact collide(const Circle &a, const Circle &b);
	Contact collide(const Aabb &a, const Circle &b);

} // namespace le
