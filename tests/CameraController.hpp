#pragma once
#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Logger.hpp>
#include <engine/core/ecs/components/ScriptComponent.hpp>
#include <engine/core/ecs/components/Camera.hpp>
#include <engine/core/events/EventSystem.hpp>

class CameraController : public ScriptComponent
{
public:
	void Update() override
	{
		auto &transform = GetComponent<Transform>();

		static float targetX = 640.0f;
		static bool movingRight = true;

		float currentX = transform.Position.x();
		float speed = 400.0f * utils::Time::DeltaTime();

		if (movingRight)
		{
			if (currentX < targetX)
			{
				transform.Position.x() += speed;
			}
			else
			{
				movingRight = false; // Меняем направление
			}
		}
		else
		{
			if (currentX > -targetX)
			{
				transform.Position.x() -= speed;
			}
			else
			{
				movingRight = true; // Меняем направление
			}
		}
	}
};
