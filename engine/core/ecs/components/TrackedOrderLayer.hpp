#pragma once
#include <functional>

/*
class TrackedOrderLayer
{
private:
	short value;
	std::function<void()> onChange;

	void NotifyChange()
	{
		if (onChange)
			onChange();
	}

public:
	TrackedOrderLayer(short initialValue, std::function<void()> callback)
		: value(initialValue), onChange(std::move(callback)) {}

	TrackedOrderLayer &operator=(short newValue)
	{
		if (value != newValue)
		{
			value = newValue;
			NotifyChange();
		}
		return *this;
	}

	operator short() const { return value; }

	short get() const { return value; }

	bool operator==(short other) const { return value == other; }
	bool operator!=(short other) const { return value != other; }
	bool operator<(short other) const { return value < other; }
	bool operator>(short other) const { return value > other; }
	bool operator<=(short other) const { return value <= other; }
	bool operator>=(short other) const { return value >= other; }
};
*/
