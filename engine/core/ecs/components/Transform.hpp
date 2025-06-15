#pragma once
#include <engine/core/ecs/components/Position2D.hpp>
#include <engine/engineapi.hpp>

struct Transform
{
private:
	bool changed = false;

	void OnModify()
	{
		changed = true;
	}

public:
	Position2D Position{glm::vec2(0.0f, 0.0f), [this]()
						{ OnModify(); }};
	glm::vec2 Scale{50.0f, 50.0f};
	glm::vec2 Origin{0.0f, 0.0f};
	float Rotation = 0.0f;

	bool GetChanged() const { return changed; }
	void SetChanged(bool newChanged) { changed = newChanged; }
};
