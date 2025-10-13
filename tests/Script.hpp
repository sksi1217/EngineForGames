#pragma once
#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Logger.hpp>
#include <engine/core/ecs/components/CoreComponents.hpp>
#include <engine/core/scene/Object.hpp>

class Script : public ScriptComponent
{
private:
public:
	entt::entity targetEntity = entt::null;

	void Update() override
	{
		if (InputManager::Get().WasKeyPressed(GLFW_KEY_SPACE))
		{
			if (targetEntity != entt::null && registry)
			{
				Object target(*registry, targetEntity);
				if (target.IsActive())
				{
					target.SetActive(false);
				}
				else
				{
					target.SetActive(true);
				}
			}
		}
	}
};
