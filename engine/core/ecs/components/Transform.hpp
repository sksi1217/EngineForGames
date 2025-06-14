#pragma once
#include <engine/core/ecs/components/TrackedVec2.hpp>

struct Transform
{
private:
	bool changed = false;

	void OnModify()
	{
		changed = true;
	}

public:
	TrackedVec2 Position{glm::vec2(0.0f, 0.0f), [this]()
						 { OnModify(); }};
	TrackedVec2 Scale{glm::vec2(50.0f, 50.0f), [this]()
					  { OnModify(); }};
	TrackedVec2 Origin{glm::vec2(0.0f, 0.0f), [this]()
					   { OnModify(); }};
	float Rotation = 0.0f;

	bool GetChanged() const { return changed; }
	void SetChanged(bool newChanged) { changed = newChanged; }
};
