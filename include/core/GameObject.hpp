#pragma once
#include <libs/entt/entt.hpp>
#include <include/components/Components.hpp>

#include <vector>
#include <memory>
#include <include/components/Components.hpp>
#include <include/utils/Logger.hpp>

struct ScriptComponentRef
{
	std::shared_ptr<ScriptComponent> script;
};

struct ScriptsContainerComponent
{
	std::vector<std::shared_ptr<ScriptComponent>> scripts;
};

class GameObject
{
public:
	entt::entity entity;

	explicit GameObject(entt::registry &registry) : registry(&registry)
	{
		entity = registry.create();
		registry.emplace<TransformComponent>(entity);
	}

	static GameObject Create(entt::registry &registry)
	{
		return GameObject(registry);
	}

	// !
	void SetActive(bool active)
	{
		if (active == isActive)
			return;

		isActive = active;

		if (isActive)
		{
			auto &scriptRef = registry->get<ScriptComponentRef>(entity);
			scriptRef.script->OnEnable(); // Вызываем OnEnable()
		}
		else if (!isActive)
		{
			auto &scriptRef = registry->get<ScriptComponentRef>(entity);
			scriptRef.script->OnDisable(); // Вызываем OnDisable()
		}
	}

	bool IsActive() const { return isActive; }

	template <typename T, typename... Args>
	T &AddComponent(Args &&...args)
	{
		if (!registry->valid(entity))
		{
			utils::Logger::error("Trying to add component to invalid entity!");
			throw std::runtime_error("Invalid entity");
		}

		if (registry->all_of<T>(entity))
		{
			utils::Logger::error("Component already exists!");
			return registry->get<T>(entity);
		}

		return registry->emplace<T>(entity, std::forward<Args>(args)...);
	}

	template <typename T>
	void RemoveComponent()
	{
		registry->remove<T>(entity);
	}

	template <typename T>
	bool HasComponent() const
	{
		return registry->all_of<T>(entity);
	}

	template <typename T>
	T &GetComponent()
	{
		return registry->get<T>(entity);
	}

	/*
	template <typename T, typename... Args>
	std::shared_ptr<T> AddScript(Args &&...args)
	{
		static_assert(std::is_base_of_v<ScriptComponent, T>, "T must inherit from ScriptComponent");

		if (!registry->valid(entity))
		{
			utils::Logger::error("Trying to add script to invalid entity!");
			throw std::runtime_error("Invalid entity");
		}

		// Проверяем, есть ли уже скрипт этого типа
		if (registry->any_of<ScriptComponentRef>(entity))
		{
			utils::Logger::error("Entity already has a script component.");
			return nullptr;
		}

		auto script = std::make_shared<T>(std::forward<Args>(args)...);

		script->entity = entity;
		script->registry = registry;

		script->Start();

		registry->emplace<ScriptComponentRef>(entity, ScriptComponentRef{script});
		return script;
	}
	*/

	template <typename T, typename... Args>
	void AddScript(Args &&...args)
	{
		static_assert(std::is_base_of_v<ScriptComponent, T>, "T must inherit from ScriptComponent");

		if (!HasComponent<ScriptsContainerComponent>())
		{
			AddComponent<ScriptsContainerComponent>();
		}

		auto &container = GetComponent<ScriptsContainerComponent>();
		auto script = std::make_shared<T>(std::forward<Args>(args)...);
		script->entity = entity;
		script->registry = registry;
		container.scripts.push_back(script);

		script->Start();
	}

	void UpdateScripts()
	{
		if (HasComponent<ScriptsContainerComponent>())
		{
			auto &container = GetComponent<ScriptsContainerComponent>();
			for (auto &script : container.scripts)
			{
				script->Update();
			}
		}
	}

private:
	entt::registry *registry;

	// !
	bool isActive = true;
};
