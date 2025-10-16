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
			transform().position.y -= speed;
		}
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_DOWN))
		{
			transform().position.y += speed;
		}
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_RIGHT))
		{
			transform().position.x += speed;
		}
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_LEFT))
		{
			transform().position.x -= speed;
		}
	}
};
