#pragma once

#include <engine/core/utils/Time.hpp>
#include <engine/core/utils/Logger.hpp>
#include <engine/core/ecs/components/CoreComponents.hpp>
#include <engine/core/scene/Object.hpp>

class Script : public ScriptComponent
{
private:
public:
	void OnEnable() override
	{
		utils::Logger::info("OnEnable");
	}
	void OnDisable() override
	{
		utils::Logger::info("OnDisable");
	}
	void OnDestroy() override
	{
		utils::Logger::info("OnDestroy");
	}
};
