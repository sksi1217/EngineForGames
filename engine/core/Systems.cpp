#include <engine/core/Systems.hpp>
#include <engine/core/ui/ImGuiContext.hpp>
#include <engine/core/ecs/components/SpriteRenderer.hpp>

// ! Система рендера объектов

void CameraSystem::Update(entt::registry &registry, Renderer &renderer)
{
	auto view = registry.view<Transform, Camera>();
	for (auto entity : view)
	{
		auto [transform, camera] = view.get<Transform, Camera>(entity);
		if (camera.isMain)
		{
			// Обновляем позицию камеры на основе transform
			Camera adjustedCamera = camera;
			// adjustedCamera.offset = transform.Position;
			renderer.SetCamera(adjustedCamera, transform);
			return;
		}
	}

	// Камера по умолчанию
	static Camera defaultCamera;
	// Transform transform;
	// renderer.SetCamera(defaultCamera, *transform);
}

void RenderSystem::Update()
{
	auto &registry = ECS::Get().GetRegistry();
	auto &renderer = Renderer::Get();

	// Обновляем камеру
	CameraSystem cameraSystem;
	cameraSystem.Update(registry, renderer);

	// Получаем все объекты со SpriteRenderer и Transform
	auto view = registry.view<Transform, SpriteRenderer>();

	// Копируем только видимые сущности и сортируем их
	std::vector<entt::entity> visibleEntities;

	for (auto entity : view)
	{
		const auto &transform = registry.get<Transform>(entity);

		if (!renderer.IsVisible(transform))
			continue;

		visibleEntities.push_back(entity);
	}

	// Сортируем только видимые объекты по OrderLayer
	std::sort(visibleEntities.begin(), visibleEntities.end(), [&](auto lhs, auto rhs)
			  { return registry.get<SpriteRenderer>(lhs).OrderLayer < registry.get<SpriteRenderer>(rhs).OrderLayer; });

	// Рендерим в отсортированном порядке
	for (auto entity : visibleEntities)
	{
		const auto &transform = registry.get<Transform>(entity);
		const auto &sprite = registry.get<SpriteRenderer>(entity);

		Renderer::RenderParams params;
		params.Position = transform.Position;
		params.Scale = transform.Scale;
		params.Rotation = transform.Rotation;
		params.Color = sprite.Color;
		params.Origin = transform.Origin;

		if (sprite.Sprite)
		{
			renderer.Render(*sprite.Sprite, params);
		}
	}
}

// ! Система передвижения объектов
void MovementSystem::Update(SpatialPartitioning &spatialPartitioning)
{
	auto &registry = ECS::Get().GetRegistry();
	auto view = registry.view<Transform, ActiveComponent>();

	view.each([&](auto entity, Transform &transform, const ActiveComponent &active)
			  {
		if (!active.isActive)
		{
			return;
		}

		if (transform.GetChanged())
		{
			spatialPartitioning.UpdateObjectPosition(entity, transform.Position);
			transform.SetChanged(false); // ! Сбрасываем флаг
		} });
}

// ! Система скриптов
void ScriptSystem::Update()
{
	auto &registry = ECS::Get().GetRegistry();
	auto view = registry.view<ScriptsContainerComponent, ActiveComponent>();

	// for (auto entity : view)

	view.each([&](auto entity, ScriptsContainerComponent &container, ActiveComponent &active)
			  {
		if (!active.isActive)
			return;

		// ! Обновляем все скрипты для этой сущности
		for (auto &script : container.scripts)
		{
			if (script->IsEnabled())
			{
				script->InternalUpdate();
			}
		} });
}

void ScriptSystem::FixedUpdate()
{
	auto &registry = ECS::Get().GetRegistry();
	auto view = registry.view<ScriptsContainerComponent, ActiveComponent>();

	view.each([&](auto entity, ScriptsContainerComponent &container, ActiveComponent &active)
			  {
		if (!active.isActive)
		{
			return;
		}

		for (auto &script : container.scripts)
		{
			if (script->IsEnabled())
			{
				script->FixedUpdate();
			}
		} });
}

// ! Система колизии объектов
void CollisionSystem::Update(SpatialPartitioning &spatialPartitioning)
{
	auto &registry = ECS::Get().GetRegistry();
	auto view = registry.view<Transform, ActiveComponent>();

	view.each([&](auto entity, const Transform &transform, const ActiveComponent &active)
			  {
		if (!active.isActive) return;


		auto nearbyEntities = spatialPartitioning.GetNearbyObjects(transform.Position);

		for (auto otherEntity : nearbyEntities)
		{
			if (entity == otherEntity)
				continue;

			const Transform &otherTransform = registry.get<Transform>(otherEntity);

			// TODO: Проверка коллизии между transform и otherTransform
		} });
}
