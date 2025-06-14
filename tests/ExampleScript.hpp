#pragma once
// #include <engine/core/components/Components.hpp>
#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Logger.hpp>
#include <engine/core/scene/GameObject.hpp>
#include <engine/core/ecs/components/SpriteRenderer.hpp>

class ExampleScript : public ScriptComponent
{
private:
	glm::vec2 velocity;
	glm::vec2 screenBounds; // Границы экрана

public:
	void Start() override
	{
		utils::Logger::info("Start");

		// Устанавливаем случайное начальное направление
		velocity = glm::vec2(100.0f, 100.0f);

		// Получаем компонент трансформации
		auto &transform = GetComponent<Transform>();

		// transform.MarkChanged();

		// Устанавливаем границы экрана (замените на реальные размеры вашего окна)
		screenBounds = glm::vec2(800.0f, 600.0f);
	}

	void Update() override
	{
		/*
		auto &transform = GetComponent<Transform>();
		auto &render = GetComponent<SpriteRenderer>();

		// Обновляем позицию
		transform.Position += velocity * utils::Time::DeltaTime();

		if (render.OrderLayer == 1)
		{
			render.OrderLayer = -1;
		}
		else if (render.OrderLayer == -1)

		{
			render.OrderLayer = 1;
		}

		// Проверяем столкновение с границами экрана
		// Правая граница
		if (transform.Position.x() + transform.Scale.x() > screenBounds.x)
		{
			transform.Position.x() = screenBounds.x - transform.Scale.x();
			velocity.x = -velocity.x;
		}
		// Левая граница
		else if (transform.Position.x() < 0)
		{
			transform.Position.x() = 0;
			velocity.x = -velocity.x;
		}

		// Нижняя граница
		if (transform.Position.y() + transform.Scale.y() > screenBounds.y)
		{
			transform.Position.y() = screenBounds.y - transform.Scale.y();
			velocity.y = -velocity.y;
		}
		// Верхняя граница
		else if (transform.Position.y() < 0)
		{
			transform.Position.y() = 0;
			velocity.y = -velocity.y;
		}
		*/
	}
};
