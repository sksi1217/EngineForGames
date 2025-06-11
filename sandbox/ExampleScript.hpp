#pragma once
#include <include/components/Components.hpp>
#include <include/utils/Time.hpp>
#include <components/Components.hpp>
#include <include/utils/Logger.hpp>
#include <include/core/GameObject.hpp>

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
		auto &transform = GetComponent<TransformComponent>();

		transform.MarkChanged();

		// Устанавливаем границы экрана (замените на реальные размеры вашего окна)
		screenBounds = glm::vec2(800.0f, 600.0f);
	}

	void Update() override
	{
		auto &transform = GetComponent<TransformComponent>();

		// Обновляем позицию
		transform.position += velocity * utils::Time::DeltaTime();
		transform.MarkChanged();

		// Проверяем столкновение с границами экрана
		// Правая граница
		if (transform.position.x + transform.scale.x > screenBounds.x)
		{
			transform.position.x = screenBounds.x - transform.scale.x;
			velocity.x = -velocity.x;
		}
		// Левая граница
		else if (transform.position.x < 0)
		{
			transform.position.x = 0;
			velocity.x = -velocity.x;
		}

		// Нижняя граница
		if (transform.position.y + transform.scale.y > screenBounds.y)
		{
			transform.position.y = screenBounds.y - transform.scale.y;
			velocity.y = -velocity.y;
		}
		// Верхняя граница
		else if (transform.position.y < 0)
		{
			transform.position.y = 0;
			velocity.y = -velocity.y;
		}
	}
};
