#pragma once
#include <extern/entt/entt.hpp>

#include <vector>
#include <memory>

#include <engine/core/utils/Logger.hpp>
#include <engine/core/ecs/components/ScriptComponent.hpp>
#include <engine/core/ecs/components/Transform.hpp>
#include <engine/core/ecs/components/ActiveComponent.hpp>
#include <engine/core/ecs/components/NameComponent.hpp>
#include <engine/core/ecs/components/TagComponent.hpp>
#include <engine/core/ecs/components/LayerRender.hpp>

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
		// ! Добавления стандартных компонентов для GameObject
		registry.emplace<Transform>(entity);
		registry.emplace<ActiveComponent>(entity);
		registry.emplace<LayerRender>(entity);
		registry.emplace<NameComponent>(entity, NameComponent{"Game Object"});
	}

	static GameObject Create(entt::registry &registry)
	{
		return GameObject(registry);
	}

	// **
	// ! Установить имя
	void SetName(const std::string &name)
	{
		auto &nc = registry->get_or_emplace<NameComponent>(entity);
		nc.name = name;
	}

	std::string GetName() const
	{
		if (registry->any_of<NameComponent>(entity))
			return registry->get<NameComponent>(entity).name;
		return "Game Object";
	}

	// Установить тег
	void SetTag(const std::string &tag)
	{
		auto &tc = registry->get_or_emplace<TagComponent>(entity);
		tc.tag = tag;
	}

	std::string GetTag() const
	{
		if (registry->any_of<TagComponent>(entity))
			return registry->get<TagComponent>(entity).tag;
		return "";
	}

	// Установить активность
	void SetActive(bool active)
	{
		auto &ac = registry->get_or_emplace<ActiveComponent>(entity);
		if (ac.isActive == active)
			return;

		ac.isActive = active;

		// Вызываем OnEnable / OnDisable у скриптов
		if (registry->any_of<ScriptsContainerComponent>(entity))
		{
			auto &scripts = registry->get<ScriptsContainerComponent>(entity);
			for (auto &script : scripts.scripts)
			{
				if (active)
					script->OnEnable();
				else
					script->OnDisable();
			}
		}
	}

	bool IsActive() const
	{
		if (registry->any_of<ActiveComponent>(entity))
			return registry->get<ActiveComponent>(entity).isActive;
		return false;
	}

	/*
	// ! Найти все объекты с определенным тегом
	static std::vector<GameObject> FindGameObjectWithTag(entt::registry &registry, const std::string &tag)
	{
		std::vector<GameObject> result;
		auto view = registry.view<TagComponent>();
		for (auto entity : view)
		{
			if (view.get<TagComponent>(entity).tag == tag)
			{
				GameObject go;
				go.entity = entity;
				go.registry = &registry;
				result.push_back(go);
			}
		}
		return result;
	}

	// ! Найти первый объект с таким именем
	static GameObject FindObjectByName(entt::registry &registry, const std::string &name)
	{
		auto view = registry.view<NameComponent>();
		for (auto entity : view)
		{
			if (view.get<NameComponent>(entity).name == name)
			{
				GameObject go;
				go.entity = entity;
				go.registry = &registry;
				return go;
			}
		}

		GameObject empty;
		empty.entity = entt::null;
		empty.registry = &registry;
		return empty; // invalid
	}
	*/
	// ***

	// ! Методы для компонентов
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

private:
	entt::registry *registry;
};
