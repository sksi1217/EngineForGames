	#pragma once

	#include <engine/engineapi.hpp>

	struct Transform;
	struct ActiveComponent;
	struct LayerRender;
	struct NameComponent;
	struct ScriptComponent;

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

		static GameObject CreateObject(entt::registry &registry)
		{
			return GameObject(registry);
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

			script->Start();
		}

	private:
		entt::registry *registry;
	};
