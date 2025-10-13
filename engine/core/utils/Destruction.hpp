// Destruction.hpp
#pragma once

#include <engine/core/scene/Object.hpp>
#include <engine/core/utils/Logger.hpp>

// Объявляем структуру таймера (минимально)
struct DestroyTimer
{
	float timeLeft = 0.0f;
};

// Объявляем функции (без реализации!)
void Destroy(const Object &obj);
void Destroy(const Object &obj, float delay);
