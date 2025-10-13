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

			if (render->OrderLayer == 0)
			{
				render->OrderLayer = -1;
			}
			else if (render->OrderLayer == -1)
			{
				render->OrderLayer = 1;
			}
			else if (render->OrderLayer == 1)
			{
				render->OrderLayer = -1;
			}

			utils::Logger::info("OrderLayer: " + std::to_string(static_cast<int>(render->OrderLayer)));
		}

		if (InputManager::Get().WasKeyPressed(GLFW_KEY_F))
		{
			utils::Logger::info("Start destroy the Object!");
			// Уничтожить себя через 5 секунд
			Destroy(gameObject(), 5.0f);
		}
	}
};
