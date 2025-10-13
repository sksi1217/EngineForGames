// Destruction.cpp
#include "Destruction.hpp"
#include <engine/core/ecs/components/CoreComponents.hpp>
#include <engine/core/ecs/components/ScriptComponent.hpp>

void Destroy(const Object &obj)
{
	auto &registry = obj.GetRegistry();
	auto entity = obj.entity;

	if (registry.any_of<ScriptsContainerComponent>(entity))
	{
		auto &scripts = registry.get<ScriptsContainerComponent>(entity);
		for (auto &script : scripts.scripts)
		{
			script->OnDestroy();
		}
	}

	registry.destroy(entity);

	utils::Logger::info("Object is destroy!");
}

void Destroy(const Object &obj, float delay)
{
	if (delay <= 0.0f)
	{
		Destroy(obj);
		utils::Logger::info("Object is destroy!");
		return;
	}

	auto &registry = obj.GetRegistry();
	auto entity = obj.entity;
	registry.emplace_or_replace<DestroyTimer>(entity, DestroyTimer{delay});
}
