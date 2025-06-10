#include <include/core/GameObject.hpp>

GameObject::GameObject(entt::registry &registry)
	: registry(&registry)
{
	entity = registry.create();
	registry.emplace<TransformComponent>(entity);
}

GameObject GameObject::Create(entt::registry &registry)
{
	return GameObject(registry);
}
