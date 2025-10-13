#pragma once

#include <engine/LightEngine.hpp>

#include <engine/core/ecs/components/ScriptComponent.hpp>

#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Logger.hpp>

class CameraController : public ScriptComponent
{
public:
	void Update() override
	{
		float speed = 400.0f * utils::Time::DeltaTime();

		if (InputManager::Get().IsKeyPressed(GLFW_KEY_UP))
		{
			transform().Position.y() -= speed;
		}
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_DOWN))
		{
			transform().Position.y() += speed;
		}
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_RIGHT))
		{
			transform().Position.x() += speed;
		}
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_LEFT))
		{
			transform().Position.x() -= speed;
		}
	}
};
