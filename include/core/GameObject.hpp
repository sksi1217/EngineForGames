#pragma once
#include <libs/entt/entt.hpp>
#include <include/core/components/SpriteComponent.hpp>
#include <include/core/components/TransformComponent.hpp>
class GameObject
{
public:
	entt::entity entity;

	explicit GameObject(entt::registry &registry);
	static GameObject Create(entt::registry &registry);

	template <typename T, typename... Args>
	T &AddComponent(Args &&...args)
	{
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

private:
	entt::registry *registry;
};
