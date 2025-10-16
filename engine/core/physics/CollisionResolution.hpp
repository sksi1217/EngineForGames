// engine/core/physics/CollisionResolution.hpp
#pragma once
#include <glm/vec2.hpp>

namespace le
{

	/**
	 * @brief Разрешает столкновение между двумя телами с учётом массы, упругости и трения.
	 *
	 * @param posA, posB — позиции центров масс (могут корректироваться для разрешения проникновения)
	 * @param velA, velB — скорости (изменяются!)
	 * @param invMassA, invMassB — обратные массы (1/mass). Для статических тел = 0.
	 * @param normal — нормаль столкновения (от B к A, единичный вектор)
	 * @param penetration — глубина проникновения (> 0, если есть пересечение)
	 * @param restitution — коэффициент упругости [0, 1]
	 * @param friction — коэффициент трения [0, ∞)
	 * @param dt — дельта времени (для коррекции позиции)
	 */
	void resolveCollision(
		glm::vec2 &posA, glm::vec2 &posB,
		glm::vec2 &velA, glm::vec2 &velB,
		float invMassA, float invMassB,
		const glm::vec2 &normal,
		float penetration,
		float restitution,
		float friction,
		float dt);

	void resolveVelocity(
		glm::vec2 &velA, glm::vec2 &velB,
		float invMassA, float invMassB,
		const glm::vec2 &normal,
		float penetration,
		float restitution,
		float friction,
		float dt);

	// Position resolver — изменяет позиции
	void resolvePosition(
		glm::vec2 &posA, glm::vec2 &posB,
		float invMassA, float invMassB,
		const glm::vec2 &normal,
		float penetration,
		const glm::vec2 &contactPoint // необязательно, но полезно для точности
	);

} // namespace le
