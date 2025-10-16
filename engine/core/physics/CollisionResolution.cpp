// engine/core/physics/CollisionResolution.cpp
#include "CollisionResolution.hpp"
#include <glm/glm.hpp>
#include <algorithm>

namespace le
{

	// version 0.2
	void resolveVelocity(
		glm::vec2 &velA, glm::vec2 &velB,
		float invMassA, float invMassB,
		const glm::vec2 &normal,
		float penetration,
		float restitution,
		float friction,
		float dt)
	{
		const float k_slop = 0.005f;
		const float k_biasFactor = 0.2f;
		const float k_maxBias = 10.0f;

		float bias = 0.0f;
		if (penetration > k_slop)
		{
			bias = -k_biasFactor * (penetration - k_slop) / dt;
			bias = std::min(bias, -k_maxBias);
		}

		glm::vec2 relVel = velA - velB;
		float velAlongNormal = glm::dot(relVel, normal);

		if (velAlongNormal > 0.0f)
			return;

		float invMassSum = invMassA + invMassB;
		if (invMassSum == 0.0f)
			return;

		float j = -(1.0f + restitution) * velAlongNormal + bias;
		j /= invMassSum;
		if (j < 0.0f)
			j = 0.0f;

		glm::vec2 impulse = normal * j;
		velA += impulse * invMassA;
		velB -= impulse * invMassB;

		// Трение
		if (friction > 0.0f)
		{
			glm::vec2 tangent = relVel - normal * velAlongNormal;
			float len2 = glm::dot(tangent, tangent);
			if (len2 > 0.0f)
			{
				tangent *= (1.0f / std::sqrt(len2));
				float jt = -glm::dot(relVel, tangent) / invMassSum;
				float maxFriction = friction * j;
				jt = std::clamp(jt, -maxFriction, maxFriction);

				glm::vec2 frictionImpulse = tangent * jt;
				velA += frictionImpulse * invMassA;
				velB -= frictionImpulse * invMassB;
			}
		}
	}

	void resolvePosition(
		glm::vec2 &posA, glm::vec2 &posB,
		float invMassA, float invMassB,
		const glm::vec2 &normal,
		float penetration,
		const glm::vec2 & /*contactPoint*/)
	{
		const float k_slop = 0.005f;
		const float k_percent = 0.4f;		// мягкое раздвижение
		const float k_maxCorrection = 0.2f; // макс. смещение за кадр

		if (penetration <= k_slop)
			return;

		float correction = (penetration - k_slop) * k_percent;
		if (correction > k_maxCorrection)
			correction = k_maxCorrection;

		glm::vec2 correctionVec = normal * correction;
		float totalInvMass = invMassA + invMassB;

		if (totalInvMass > 0.0f)
		{
			posA += correctionVec * (invMassA / totalInvMass);
			posB -= correctionVec * (invMassB / totalInvMass);
		}
		// Если оба статические — ничего не делаем
	}

	//
	void resolveCollision(
		glm::vec2 &posA, glm::vec2 &posB, // позиции НЕ изменяются напрямую
		glm::vec2 &velA, glm::vec2 &velB,
		float invMassA, float invMassB,
		const glm::vec2 &normal,
		float penetration,
		float restitution,
		float friction,
		float dt)
	{
		// === 1. Baumgarte stabilization с slop (как в Box2D) ===
		const float k_slop = 0.005f;	 // допустимое проникновение (5 мм)
		const float k_biasFactor = 0.2f; // коэффициент Baumgarte (0.1–0.3)
		const float k_maxBias = 10.0f;	 // макс. скорость коррекции (м/с)

		float bias = 0.0f;
		if (penetration > k_slop)
		{
			bias = -k_biasFactor * (penetration - k_slop) / dt;
			bias = std::min(bias, -k_maxBias); // ограничиваем, чтобы не было перекоррекции
		}

		// === 2. Импульс отскока + коррекция через bias ===
		glm::vec2 relVel = velA - velB;
		float velAlongNormal = glm::dot(relVel, normal);

		// Если тела удаляются — ничего не делаем
		if (velAlongNormal > 0.0f)
		{
			return;
		}

		float invMassSum = invMassA + invMassB;
		if (invMassSum == 0.0f)
		{
			return; // оба статические
		}

		// Полный импульс: включает restitution И bias для коррекции проникновения
		float j = -(1.0f + restitution) * velAlongNormal + bias;
		j /= invMassSum;

		// Импульс не может быть отрицательным (тела не могут "прилипать" без трения)
		if (j < 0.0f)
			j = 0.0f;

		glm::vec2 impulse = normal * j;
		velA += impulse * invMassA;
		velB -= impulse * invMassB;

		// === 3. Трение (tangential impulse) ===
		if (friction > 0.0f)
		{
			glm::vec2 tangent = relVel - normal * velAlongNormal;
			float tangentLen2 = glm::dot(tangent, tangent);

			if (tangentLen2 > 0.0f)
			{
				tangent *= (1.0f / std::sqrt(tangentLen2)); // normalize

				float jt = -glm::dot(relVel, tangent) / invMassSum;

				// Ограничиваем трение по закону Кулона: |jt| <= j * friction
				float maxFriction = friction * j;
				jt = std::clamp(jt, -maxFriction, maxFriction);

				glm::vec2 frictionImpulse = tangent * jt;
				velA += frictionImpulse * invMassA;
				velB -= frictionImpulse * invMassB;
			}
		}

		// Позиции НЕ корректируются здесь — это делает position solver в Box2D,
		// но в простом движке мы полагаемся на Baumgarte + итерации.
		// Если всё ещё есть проникновение — увеличь итерации или уменьши dt.
	}

} // namespace le
