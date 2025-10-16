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
		// Получаем компонент физики
		auto &rb = GetComponent<le::Rigidbody2D>();

		// Если тело кинематическое или статическое — не управляем
		if (rb.GetKinematic() || rb.GetStatic())
		{
			// Но если хочешь управлять кинематикой — см. примечание ниже
			return;
		}

		float moveSpeed = 200.0f; // Пикселей в секунду
		glm::vec2 movement(0.0f, 0.0f);

		if (InputManager::Get().IsKeyPressed(GLFW_KEY_W))
			movement.y -= moveSpeed;
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_S))
			movement.y += moveSpeed;
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_A))
			movement.x -= moveSpeed;
		if (InputManager::Get().IsKeyPressed(GLFW_KEY_D))
			movement.x += moveSpeed;

		float maxSpeed = 500.0f; // пикселей/сек
		if (glm::length(rb.velocity) > maxSpeed)
		{
			rb.velocity = glm::normalize(rb.velocity) * maxSpeed;
		}

		// Альтернатива: прикладывать силу (для плавного ускорения)
		rb.acceleration += movement * rb.GetMass(); // если используешь F = ma

		// Управление активностью и уничтожением — без изменений
		if (InputManager::Get().WasKeyPressed(GLFW_KEY_SPACE))
		{
			if (targetEntity != entt::null && registry)
			{
				Object target(*registry, targetEntity);
				target.SetActive(!target.IsActive());
				utils::Logger::info("IsActive toggled");
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
