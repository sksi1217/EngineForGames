#pragma once
#include <engine/core/utils/Destruction.hpp>
#include <engine/core/ecs/components/CoreComponents.hpp>

#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Logger.hpp>

class ExampleScript : public ScriptComponent
{
private:
	Sprite *render = nullptr;

public:
	entt::entity targetEntity = entt::null;

	void Start() override
	{
		utils::Logger::info("Start Object: " + name());

		render = &GetComponent<Sprite>();
	}

	void Update() override
	{

		float moveSpeed = 200.0f; // Пикселей в секунду
		glm::vec2 movement(0.0f, 0.0f);

		// Управление с клавиатуры
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_W))
			movement.y -= moveSpeed;
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_S))
			movement.y += moveSpeed;
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_A))
			movement.x -= moveSpeed;
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_D))
			movement.x += moveSpeed;

		// Применяем движение
		transform().Position += movement * utils::Time::DeltaTime();

		// Переключение слоя рендера при нажатии пробела
		if (InputManager::Get().WasKeyPressed(GLFW_KEY_SPACE))
		{
			if (targetEntity != entt::null && registry)
			{
				Object target(*registry, targetEntity);
				if (target.IsActive())
				{
					target.SetActive(false);
				}
				else
				{
					target.SetActive(true);
				}

				utils::Logger::info("IsActive");
			}
		}

		if (InputManager::Get().WasKeyPressed(GLFW_KEY_F))
		{
			utils::Logger::info("Start destroy the Object!");
			if (targetEntity != entt::null && registry)
			{
				Object target(*registry, targetEntity);

				Destroy(target);
			}
		}
	}
};
