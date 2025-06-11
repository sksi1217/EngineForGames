#include <include/utils/Systems.hpp>
#include <ui/ImGuiContext.hpp>

// ! Система рендера объектов
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
		params.origin = transform.origin;

        if (sprite.texture) {
            renderer.Render(*sprite.texture, params);
        } });
}

// ! Система передвижения объектов
void MovementSystem::Update(SpatialPartitioning &spatialPartitioning)
{
	auto &registry = ECS::Get().GetRegistry();
	auto view = registry.view<TransformComponent>();

	for (auto entity : view)
	{
		auto &transform = view.get<TransformComponent>(entity);

		if (transform.changed)
		{
			spatialPartitioning.UpdateObjectPosition(entity, transform.position);
			transform.changed = false; // Сбрасываем флаг
		}
	}
}
// ! Система скриптов
void ScriptSystem::Update()
{
	auto &registry = ECS::Get().GetRegistry();

	// Ищем все сущности с ScriptsContainerComponent
	auto view = registry.view<ScriptsContainerComponent>();

	for (auto entity : view)
	{
		auto &container = view.get<ScriptsContainerComponent>(entity);

		// Обновляем все скрипты для этой сущности
		for (auto &script : container.scripts)
		{
			if (script->IsEnabled())
			{
				script->InternalUpdate();
			}
		}
	}
}

void ScriptSystem::FixedUpdate()
{
	auto &registry = ECS::Get().GetRegistry();
	auto view = registry.view<ScriptsContainerComponent>();

	for (auto entity : view)
	{
		auto &container = view.get<ScriptsContainerComponent>(entity);

		for (auto &script : container.scripts)
		{
			if (script->IsEnabled())
			{
				script->FixedUpdate();
			}
		}
	}
}

// ! Система колизии объектов
void CollisionSystem::Update(SpatialPartitioning &spatialPartitioning)
{
	auto &registry = ECS::Get().GetRegistry();
	auto view = registry.view<TransformComponent>();

	view.each([&](auto entity, auto &transform)
			  {
		auto nearbyEntities = spatialPartitioning.GetNearbyObjects(transform.position);

		for (auto otherEntity : nearbyEntities)
		{
			if (entity == otherEntity)
				continue;

			const auto &otherTransform = registry.get<TransformComponent>(otherEntity);

			// TODO: Проверка коллизии между transform и otherTransform
		} });
}
