#include <engine/core/physics/Collision.hpp>

namespace Physics
{

	bool CheckAABB(const BoxCollider2D &a, const Transform &ta,
				   const BoxCollider2D &b, const Transform &tb)
	{
		glm::vec2 minA = a.GetMin(ta);
		glm::vec2 maxA = a.GetMax(ta);

		glm::vec2 minB = b.GetMin(tb);
		glm::vec2 maxB = b.GetMax(tb);

		return (minA.x < maxB.x && maxA.x > minB.x &&
				minA.y < maxB.y && maxA.y > minB.y);
	}

}
