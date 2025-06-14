#pragma once
#include <extern/entt/entt.hpp>

struct ScriptComponent
{
protected:
	bool enabled = true;

public:
	entt::entity entity = entt::null;
	entt::registry *registry = nullptr;

	virtual ~ScriptComponent() = default;

	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}

	// ! Хук куда можно подключить логику
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual void OnDestroy() {}

	// ?
	void InternalUpdate()
	{
		if (!enabled)
			return;

		Update();
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
