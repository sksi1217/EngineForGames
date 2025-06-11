#pragma once
#include <include/components/Components.hpp>
#include <include/utils/Time.hpp>
#include <components/Components.hpp>
#include <include/utils/Logger.hpp>
#include <include/core/GameObject.hpp>

class Script : public ScriptComponent
{
private:
public:
	void Start() override
	{
	}

	void Update() override
	{
		auto &transform = GetComponent<TransformComponent>();

		if (transform.rotation >= 360)
		{
			transform.rotation = 0;
		}
		else
		{
			// ? 45 градусов в секунду
			transform.rotation += 45.0f * utils::Time::DeltaTime();
		}
	}
};
