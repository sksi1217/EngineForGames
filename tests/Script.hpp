#pragma once
#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Logger.hpp>
#include <engine/core/scene/GameObject.hpp>

class Script : public ScriptComponent
{
private:
public:
	void Start() override
	{
	}

	void Update() override
	{
		auto &transform = GetComponent<Transform>();

		if (transform.Rotation >= 360)
		{
			transform.Rotation = 0;
		}
		else
		{
			// ? 45 градусов в секунду
			transform.Rotation += 45.0f * utils::Time::DeltaTime();
		}
	}
};
