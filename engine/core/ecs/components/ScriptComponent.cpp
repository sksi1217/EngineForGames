#include <engine/core/ecs/components/ScriptComponent.hpp>
#include <engine/core/scene/Object.hpp> // Теперь можно включать — нет цикла

Object ScriptComponent::gameObject()
{
	if (registry == nullptr || entity == entt::null || !registry->valid(entity))
	{
		utils::Logger::error("ScriptComponent is not attached to a valid entity!");
		throw std::runtime_error("Invalid entity or registry");
	}
	return Object(*registry, entity);
}
