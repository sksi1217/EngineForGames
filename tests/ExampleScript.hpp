#pragma once
// #include <engine/core/components/Components.hpp>

#include <engine/engineapi.hpp>

#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Logger.hpp>
#include <engine/core/scene/GameObject.hpp>
#include <engine/core/events/InputManager.hpp>
// #include <engine/core/ecs/components/SpriteRenderer.hpp>

class ExampleScript : public ScriptComponent
{
private:
	glm::vec2 velocity;
	glm::vec2 screenBounds; // Границы экрана

public:
	void Start() override
	{
		utils::Logger::info("Start");

		velocity = glm::vec2(0.0f, 0.0f);
		screenBounds = glm::vec2(800.0f, 600.0f);
	}

	void Update() override
	{
		auto &transform = GetComponent<Transform>();
		auto &render = GetComponent<Sprite>();

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
		transform.Position += movement * utils::Time::DeltaTime();

		// Переключение слоя рендера при нажатии пробела
		if (InputManager::Get().WasKeyPressed(GLFW_KEY_SPACE))
		{
			render.OrderLayer = -render.OrderLayer;
		}

		/*
		// Ограничения движения внутри окна
		if (transform.Position.x() < 0)
			transform.Position.x() = 0;
		if (transform.Position.y() < 0)
			transform.Position.y() = 0;
		if (transform.Position.x() + transform.Scale.x > screenBounds.x)
			transform.Position.x() = screenBounds.x - transform.Scale.x;
		if (transform.Position.y() + transform.Scale.y > screenBounds.y)
			transform.Position.y() = screenBounds.y - transform.Scale.y;
		*/
	}
};
