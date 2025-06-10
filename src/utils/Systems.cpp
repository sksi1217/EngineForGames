#include <include/utils/Systems.hpp>

void RenderSystem::Update()
{
	auto &registry = ECS::Get().GetRegistry();
	auto &renderer = Renderer::Get();

	// Правильный способ использования group в EnTT
	auto group = registry.group<SpriteComponent>(entt::get<TransformComponent>);

	group.each([&renderer](auto entity, auto &sprite, auto &transform)
			   {
        Renderer::RenderParams params;
        params.position = transform.position;
        params.size = transform.scale;
        params.rotation = transform.rotation;
        params.color = sprite.color;
        params.origin = sprite.origin;

        if (sprite.texture) {
            renderer.Render(*sprite.texture, params);
        } });
}

void MovementSystem::Update(SpatialPartitioning &spatialPartitioning)
{

	auto &registry = ECS::Get().GetRegistry();

	auto view = registry.view<TransformComponent>();

	for (auto entity : view)
	{
		auto &transform = view.get<TransformComponent>(entity);
		glm::vec2 oldPos = transform.position;

		if (transform.position != oldPos)
		{
			spatialPartitioning.UpdateObjectPosition(entity, oldPos);
		}
	}
}
