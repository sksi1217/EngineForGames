#pragma once
#include <engine/core/graphics/textures/Texture2D.hpp>
#include <glm/glm.hpp>
#include <extern/entt/entt.hpp>

struct SpriteRenderer
{
public:
	Texture2D *Sprite;
	glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
	bool FlipX = false;
	bool FlipY = false;
	glm::vec2 SpriteOffset{0.0f, 0.0f};
	glm::vec2 SpriteSize{0.0f, 0.0f};
	short OrderLayer = 0;
};

/*
struct RenderOrder
{
	static inline bool needsResort = true;
	// ! Методы управления
	static void MarkChanged()
	{
		needsResort = true;
	}
};

struct SpriteRenderer
{
public:
	Texture2D *Sprite;
	glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
	bool FlipX = false;
	bool FlipY = false;
	glm::vec2 SpriteOffset{0.0f, 0.0f};
	glm::vec2 SpriteSize{0.0f, 0.0f};
	TrackedOrderLayer OrderLayer{0, []()
								 { RenderOrder::MarkChanged(); }};
};

struct RenderOrderManager
{
	// ! Данные для сортировки
	static inline std::vector<std::pair<short, entt::entity>> renderQueue;


	// ! Методы управления
	static void MarkChanged()
	{
		needsResort = true;
	}


static void Clear()
{
	renderQueue.clear();
	RenderOrder::needsResort = true;
}

static void SortIfNeeded(entt::registry &registry)
{
	if (!RenderOrder::needsResort)
		return;

	renderQueue.clear();
	auto view = registry.view<SpriteRenderer, Transform, ActiveComponent>();
	view.each([](auto entity, auto &sprite, auto &, auto &active)
			  {
			if (active.isActive) {
				renderQueue.emplace_back(sprite.OrderLayer, entity);
			} });

	std::stable_sort(renderQueue.begin(), renderQueue.end());
	RenderOrder::needsResort = false;
}
}
;
*/
