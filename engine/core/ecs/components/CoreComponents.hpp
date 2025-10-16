#pragma once

// #include <engine/core/ecs/components/Position2D.hpp>
#include <glm/glm.hpp>
#include <string>

class Object;

// ! Имя объекта
struct NameComponent
{
	std::string name;

	// Конструктор по умолчанию
	NameComponent() = default;

	// Конструктор от строки
	explicit NameComponent(const std::string &n) : name(n) {}
	explicit NameComponent(const char *n) : name(n) {}

private:
	const bool enable = true;
};

// ! Флаг активности
struct ActiveComponent
{
	bool isActive = true;

private:
	const bool enable = true;
};

// ! Тег объекта
struct TagComponent
{
	std::string tag;

private:
	const bool enable = true;
};

enum class RenderLayer
{
	Default,
	Effects,
	UI
};

struct LayerRender
{
	RenderLayer Layer;

private:
	const bool enable = true;
};

// ! Позиция | рамер | разварот
struct Transform
{

private:
	const bool enable = true;

public:
	glm::vec2 position{0.0f, 0.0f};

	glm::vec2 scale{50.0f, 50.0f};
	glm::vec2 origin{0.0f, 0.0f};
	float rotation = 0.0f;
};
