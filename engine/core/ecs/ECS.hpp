#pragma once
#include <extern/entt/entt.hpp>

class ECS
{
public:
	static ECS &Get()
	{
		static ECS instance;
		return instance;
	}

	entt::registry &GetRegistry() { return registry; }

private:
	entt::registry registry;
};
