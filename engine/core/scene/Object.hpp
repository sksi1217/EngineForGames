#pragma once

// #include <engine/engineapi.hpp>

#include <extern/entt/entt.hpp>
#include <vector>

#include <engine/core/ecs/components/CoreComponents.hpp>
#include <engine/core/ecs/components/ScriptComponent.hpp>

#include <engine/core/utils/Logger.hpp>

struct ScriptComponentRef
{
	std::shared_ptr<ScriptComponent> script;
};

struct ScriptsContainerComponent
{
	std::vector<std::shared_ptr<ScriptComponent>> scripts;
};

class Object
{
public:
	entt::entity entity;

	explicit Object(entt::registry &registry) : registry(&registry)
	{
		entity = registry.create();
		// ! Добавления стандартных компонентов для Object
		registry.emplace<Transform>(entity);
		registry.emplace<ActiveComponent>(entity);
		registry.emplace<LayerRender>(entity);
		registry.emplace<NameComponent>(entity, NameComponent{"GameObject"});
	}

	Object(entt::registry &registry, entt::entity ent) : registry(&registry), entity(ent)
	{
		// Проверка валидности не обязательна, но можно добавить
		if (!registry.valid(entity))
		{
			utils::Logger::error("Trying to create Object from invalid entity!");
			throw std::runtime_error("Invalid entity");
		}
	}

	static Object CreateObject(entt::registry &registry)
	{
		return Object(registry);
	}

	// ** Сетеры
	// ! Установить имя
	void SetName(const std::string &name)
	{
		auto &nc = registry->get_or_emplace<NameComponent>(entity);
		nc.name = name;
	}

	// ! Установить тег
	void SetTag(const std::string &tag)
	{
		auto &tc = registry->get_or_emplace<TagComponent>(entity);
		tc.tag = tag;
	}

	// ! Установить активность
	void SetActive(bool active)
	{
		auto &ac = registry->get_or_emplace<ActiveComponent>(entity);
		if (ac.isActive == active)
			return;

		ac.isActive = active;

		// Вызываем OnEnable / OnDisable у скриптов
		if (HasComponent<ScriptsContainerComponent>())
		{
			auto &scripts = registry->get<ScriptsContainerComponent>(entity);
			for (auto &script : scripts.scripts)
			{
				if (active)
					script->Enable();
				else
					script->Disable();
			}
		}
	}
	// ** ---

	// ** Геттеры
	std::string GetName()
	{
		if (registry->any_of<NameComponent>(entity))
			return registry->get<NameComponent>(entity).name;
		return "Game Object";
	}

	std::string GetTag()
	{
		if (registry->any_of<TagComponent>(entity))
			return registry->get<TagComponent>(entity).tag;
		return "";
	}

	// ** ---

	bool IsActive()
	{
		if (registry->any_of<ActiveComponent>(entity))
			return registry->get<ActiveComponent>(entity).isActive;
		return false;
	}

	// ***

	// ! Методы для добавления компонентов
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
	bool HasComponent()
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

		script->Awake();
		script->Start();
	}

	template <typename T>
	T &GetScript()
	{
		static_assert(std::is_base_of_v<ScriptComponent, T>, "T must inherit from ScriptComponent");

		if (!HasComponent<ScriptsContainerComponent>())
		{
			utils::Logger::error("Entity does not have any scripts!");
			throw std::runtime_error("No ScriptsContainerComponent found");
		}

		auto &container = GetComponent<ScriptsContainerComponent>();
		for (auto &script : container.scripts)
		{
			if (auto *casted = dynamic_cast<T *>(script.get()))
			{
				return *casted;
			}
		}

		utils::Logger::error("Script of type " + std::string(typeid(T).name()) + " not found!");
		throw std::runtime_error("Script not found");
	}

	entt::registry &GetRegistry() const { return *registry; }

private:
	entt::registry *registry;
};
