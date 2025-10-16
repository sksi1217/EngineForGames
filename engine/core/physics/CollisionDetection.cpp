// engine/core/physics/CollisionDetection.cpp
#include "CollisionDetection.hpp"
#include <cmath>
#include <algorithm>
#include <glm/geometric.hpp>

namespace le
{
	SweptResult sweptAABB(const Aabb &a, const glm::vec2 &velA, const Aabb &b, float dt)
	{
		SweptResult result;

		if (dt == 0.0f)
			return result;

		glm::vec2 vel = velA * dt; // перемещение за dt

		// Границы AABB
		float aMinX = a.center.x - a.halfSize.x;
		float aMaxX = a.center.x + a.halfSize.x;
		float aMinY = a.center.y - a.halfSize.y;
		float aMaxY = a.center.y + a.halfSize.y;

		float bMinX = b.center.x - b.halfSize.x;
		float bMaxX = b.center.x + b.halfSize.x;
		float bMinY = b.center.y - b.halfSize.y;
		float bMaxY = b.center.y + b.halfSize.y;

		// Расстояния до входа и выхода по X
		float xEntry, xExit;
		if (vel.x > 0.0f)
		{
			xEntry = bMinX - aMaxX;
			xExit = bMaxX - aMinX;
		}
		else if (vel.x < 0.0f)
		{
			xEntry = bMaxX - aMinX;
			xExit = bMinX - aMaxX;
		}
		else
		{
			xEntry = -std::numeric_limits<float>::max();
			xExit = std::numeric_limits<float>::max();
		}

		// Расстояния до входа и выхода по Y
		float yEntry, yExit;
		if (vel.y > 0.0f)
		{
			yEntry = bMinY - aMaxY;
			yExit = bMaxY - aMinY;
		}
		else if (vel.y < 0.0f)
		{
			yEntry = bMaxY - aMinY;
			yExit = bMinY - aMaxY;
		}
		else
		{
			yEntry = -std::numeric_limits<float>::max();
			yExit = std::numeric_limits<float>::max();
		}

		// Время входа/выхода (избегаем деления на 0)
		float tEntryX = (vel.x != 0.0f) ? xEntry / vel.x : -std::numeric_limits<float>::max();
		float tExitX = (vel.x != 0.0f) ? xExit / vel.x : std::numeric_limits<float>::max();
		float tEntryY = (vel.y != 0.0f) ? yEntry / vel.y : -std::numeric_limits<float>::max();
		float tExitY = (vel.y != 0.0f) ? yExit / vel.y : std::numeric_limits<float>::max();

		// Общее время входа и выхода
		float tEntry = std::max(tEntryX, tEntryY);
		float tExit = std::min(tExitX, tExitY);

		// Проверка: есть ли пересечение траекторий?
		if (tEntry > tExit || tEntry < 0.0f || tEntry > 1.0f)
			return result; // нет коллизии

		result.hit = true;
		result.time = tEntry;

		// Определяем нормаль
		if (tEntryX > tEntryY)
		{
			result.normal = (vel.x > 0.0f) ? glm::vec2(-1, 0) : glm::vec2(1, 0);
		}
		else
		{
			result.normal = (vel.y > 0.0f) ? glm::vec2(0, -1) : glm::vec2(0, 1);
		}

		return result;
	}

	Contact collide(const Aabb &a, const Aabb &b)
	{
		glm::vec2 minA = a.center - a.halfSize;
		glm::vec2 maxA = a.center + a.halfSize;
		glm::vec2 minB = b.center - b.halfSize;
		glm::vec2 maxB = b.center + b.halfSize;

		// float safetyMargin = 2.0f; // пикселя
		float overlapX = std::min(maxA.x, maxB.x) - std::max(minA.x, minB.x);
		float overlapY = std::min(maxA.y, maxB.y) - std::max(minA.y, minB.y);

		if (overlapX <= 0.0f || overlapY <= 0.0f)
			return {};

		Contact contact;
		contact.intersecting = true;

		if (overlapX < overlapY)
		{
			contact.penetration = overlapX;
			contact.normal = (a.center.x < b.center.x) ? glm::vec2(-1, 0) : glm::vec2(1, 0);
		}
		else
		{
			contact.penetration = overlapY;
			contact.normal = (a.center.y < b.center.y) ? glm::vec2(0, -1) : glm::vec2(0, 1);
		}

		// Точка контакта — центр пересечения
		glm::vec2 overlapMin = glm::max(minA, minB);
		glm::vec2 overlapMax = glm::min(maxA, maxB);
		contact.point = (overlapMin + overlapMax) * 0.5f;

		return contact;
	}

	Contact collide(const Circle &a, const Circle &b)
	{
		glm::vec2 delta = a.center - b.center;
		float distance = glm::length(delta);
		float radiusSum = a.radius + b.radius;

		if (distance <= 0.0f || distance >= radiusSum)
		{
			return {};
		}

		Contact c;
		c.intersecting = true;
		c.penetration = radiusSum - distance;
		c.normal = delta / distance; // нормализованный вектор от B к A
		return c;
	}

	Contact collide(const Aabb &a, const Circle &b)
	{
		// Находим ближайшую точку AABB к центру круга
		float closestX = std::clamp(b.center.x, a.center.x - a.halfSize.x, a.center.x + a.halfSize.x);
		float closestY = std::clamp(b.center.y, a.center.y - a.halfSize.y, a.center.y + a.halfSize.y);

		glm::vec2 closestPoint{closestX, closestY};
		glm::vec2 delta = b.center - closestPoint;
		float distance = glm::length(delta);

		if (distance <= 0.0f || distance >= b.radius)
		{
			return {};
		}

		Contact c;
		c.intersecting = true;
		c.penetration = b.radius - distance;
		c.normal = (distance > 0.0f) ? (delta / distance) : glm::vec2(1.0f, 0.0f);
		return c;
	}
} // namespace le
