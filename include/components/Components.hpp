#pragma once

#include <glm/glm.hpp>
#include "graphics/Texture2D.hpp"

class GameObject;

struct SpriteComponent
{
	Texture2D *texture;
	glm::vec2 textureOffset{0.0f, 0.0f};
	glm::vec2 textureSize{0.0f, 0.0f};
	glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
	bool flipX = false;
	bool flipY = false;
};

struct TransformComponent
{
	glm::vec2 position{0.0f, 0.0f};
	glm::vec2 scale{50.0f, 50.0f};
	glm::vec2 origin{0, 0};
	float rotation = 0.0f;

	// ! Флаг для отслеживания изменений
	bool changed = false;

	void MarkChanged() { changed = true; }
};

class ScriptComponent
{

protected:
	bool enabled = true;

public:
	entt::entity entity = entt::null;
	entt::registry *registry = nullptr;

	// explicit ScriptComponent() {}
	virtual ~ScriptComponent() = default;

	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void OnDestroy() {}

	// ?
	void InternalUpdate()
	{
		if (!enabled)
			return;

		Update(); // Вызываем пользовательский Update
	}

	// ?

	// ! Вызывается при начале столкновения с другим объектом (other)
	virtual void OnCollisionEnter(entt::entity other) {}
	// ! Вызывается при окончании столкновения  с другим объектом
	virtual void OnCollisionExit(entt::entity other) {}
	// ! Вызывается при входе другого объекта в триггерную область
	virtual void OnTriggerEnter(entt::entity other) {}
	// ! Вызывается при выходе другого объекта из триггерной области
	virtual void OnTriggerExit(entt::entity other) {}

	// ! Вызывается, когда скрипт включён и активен
	virtual void OnEnable() {}
	// ! Вызывается, когда скрипт выключен или объект неактивен
	virtual void OnDisable() {}

	bool IsEnabled() const { return enabled; }

	virtual void Enable()
	{
		if (!enabled)
		{
			enabled = true;
			OnEnable();
		}
	}

	virtual void Disable()
	{
		if (enabled)
		{
			enabled = false;
			OnDisable();
		}
	}

	// TODO: Поиск компонентов (тегу или имени)
	entt::entity FindEntityByName(const std::string &name);

	// TODO: Найти все сущности с определённым тегом
	std::vector<entt::entity> FindEntitiesByTag(const std::string &tag);

	// ! Проверка наличия компонента
	template <typename T>
	bool HasComponent() { return registry->all_of<T>(entity); }

	// ! Получения компонента
	template <typename T>
	T &GetComponent() { return registry->get<T>(entity); }
};
