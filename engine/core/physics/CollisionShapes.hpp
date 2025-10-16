// engine/core/physics/CollisionShapes.hpp
#pragma once
#include <glm/vec2.hpp>

namespace le
{

	struct Aabb
	{
		glm::vec2 center;	// центр прямоугольника
		glm::vec2 halfSize; // половина ширины и высоты (half-extents)
	};

	struct Circle
	{
		glm::vec2 center; // центр круга
		float radius;	  // радиус (> 0)
	};

	struct Contact
	{
		bool intersecting = false;
		glm::vec2 normal{0.0f, 1.0f};
		float penetration = 0.0f;

		glm::vec2 point;
	};

} // namespace le
