#pragma once
#include <functional>
#include <engine/engineapi.hpp>

class Position2D
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
	Position2D(glm::vec2 initialValue, std::function<void()> callback = nullptr)
		: value(initialValue), onModify(callback) {}

	operator glm::vec2() const
	{
		return value;
	}

	Position2D &operator=(glm::vec2 newValue)
	{
		if (value != newValue)
		{
			value = newValue;
			NotifyChange();
		}
		return *this;
	}

	Position2D &operator+=(glm::vec2 rhs)
	{
		value += rhs;
		NotifyChange();
		return *this;
	}

	Position2D &operator-=(glm::vec2 rhs)
	{
		value -= rhs;
		NotifyChange();
		return *this;
	}

	Position2D &operator*=(float rhs)
	{
		value *= rhs;
		NotifyChange();
		return *this;
	}

	Position2D &operator/=(float rhs)
	{
		value /= rhs;
		NotifyChange();
		return *this;
	}

	glm::vec2 operator+(const glm::vec2 &rhs) const
	{
		return value + rhs;
	}

	float x() const { return value.x; }
	float y() const { return value.y; }

	float &x() { return value.x; }
	float &y() { return value.y; }
};
