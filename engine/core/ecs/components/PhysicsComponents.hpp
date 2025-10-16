#pragma once

#include <glm/glm.hpp>
#include <algorithm>

namespace le
{

	// ============================================================================
	// Rigidbody2D — физическое тело
	// ============================================================================
	struct Rigidbody2D
	{
	public:
		glm::vec2 velocity{0.0f};	  // текущая скорость
		glm::vec2 acceleration{0.0f}; // текущее ускорение (сумма сил / масса)
		float restitution{0.8f};	  // упругость: [0.0 = полностью поглощает, 1.0 = идеальный отскок]
		float friction{0.5f};		  // трение: [0.0 = лёд, 1.0 = резина]

		// Конструктор для удобства
		Rigidbody2D(float m = 1.0f, float rest = 0.8f, float fric = 0.5f)
			: mass(m > 0.0f ? m : 0.0f), inverseMass(m > 0.0f ? 1.0f / m : 0.0f), restitution(std::clamp(rest, 0.0f, 1.0f)), friction(std::clamp(fric, 0.0f, 1.0f)), isStatic(m <= 0.0f)
		{
		}

		void SetKinematic(bool active)
		{
			isKinematic = active;

			if (isKinematic)
			{
				SetMass(0);
			}
		}

		void SetStatic(bool active)
		{
			isStatic = active;
			/*
			if (isStatic)
			{
				SetMass(0);
			}
			*/
		}

		// Обновление inverseMass при изменении массы (если будешь менять массу во время игры)
		void SetMass(float m)
		{
			if (!isKinematic || !isStatic)
			{
				mass = m > 0.0f ? m : 0.0f;
				inverseMass = m > 0.0f ? 1.0f / m : 0.0f;
				isStatic = (m <= 0.0f);
			}
		}

		bool GetKinematic()
		{
			return isKinematic;
		}

		bool GetStatic()
		{
			return isStatic;
		}

		float GetMass()
		{
			return inverseMass;
		}

	private:
		bool isKinematic{false}; // если true — не реагирует на силы, но может двигаться вручную
		bool isStatic{false};	 // если true — никогда не двигается (mass = 0)

		float mass{1.0f};		 // масса (> 0). Если 0 — тело статическое.
		float inverseMass{1.0f}; // кэшированная 1/mass (для оптимизации)
	};

	// ============================================================================
	// BoxCollider2D — прямоугольный коллайдер (AABB)
	// ============================================================================
	struct BoxCollider2D
	{
		glm::vec2 size{100.0f, 100.0f}; // ПОЛНЫЙ размер (ширина × высота)
		glm::vec2 offset{0.0f, 0.0f};	// смещение относительно Transform.position
		bool isTrigger{false};			// триггер: вызывает события, но не физику

		// Вспомогательные методы (можно вынести в утилиты, но удобно здесь)
		glm::vec2 halfSize() const
		{
			return size * 0.5f;
		}

		// Глобальные границы (в мировых координатах) — кэшируются в системе физики
		// Не храним здесь, чтобы не дублировать данные!
	};

	// ============================================================================
	// CircleCollider2D — круговой коллайдер
	// ============================================================================
	struct CircleCollider2D
	{
		float radius{0.5f};			  // радиус (> 0)
		glm::vec2 offset{0.0f, 0.0f}; // смещение относительно Transform.position
		bool isTrigger{false};

		CircleCollider2D(float r = 0.5f) : radius(std::max(r, 0.001f)) {}
	};

} // namespace le
