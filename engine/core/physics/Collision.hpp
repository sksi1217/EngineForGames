#pragma once
#include <BoxCollider2D.hpp>
#include <Transform.hpp>

// #include <engine/engineapi.hpp>

namespace Physics
{

	// Проверка коллизии AABB между двумя коллайдерами
	bool CheckAABB(const BoxCollider2D &a, const Transform &ta,
				   const BoxCollider2D &b, const Transform &tb);

	// Можно добавить другие типы коллизий позже:
	// bool CheckCircleVsCircle(...);
	// bool CheckSAT(...); // Для полигонов
}
