#pragma once
#include <glm/glm.hpp>
#include <functional>

class TrackedVec2
{
private:
	glm::vec2 value;
	std::function<void()> onModify;

	void NotifyChange()
	{
		if (onModify)
			onModify();
	}

public:
	TrackedVec2(glm::vec2 initialValue, std::function<void()> callback)
		: value(initialValue), onModify(callback) {}

	// Чтение как vec2
	operator glm::vec2() const
	{
		return value;
	}

	// Присвоение
	TrackedVec2 &operator=(glm::vec2 newValue)
	{
		if (value != newValue)
		{
			value = newValue;
			NotifyChange();
		}
		return *this;
	}

	TrackedVec2 &operator+=(glm::vec2 rhs)
	{
		value += rhs;
		NotifyChange();
		return *this;
	}

	TrackedVec2 &operator-=(glm::vec2 rhs)
	{
		value -= rhs;
		NotifyChange();
		return *this;
	}

	TrackedVec2 &operator*=(float rhs)
	{
		value *= rhs;
		NotifyChange();
		return *this;
	}

	TrackedVec2 &operator/=(float rhs)
	{
		value /= rhs;
		NotifyChange();
		return *this;
	}

	float x() const { return value.x; }
	float y() const { return value.y; }

	float &x() { return value.x; }
	float &y() { return value.y; }
};
