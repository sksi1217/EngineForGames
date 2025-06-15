#include <engine/core/Systems.hpp>

// ! Система рендера объектов

void CameraSystem::Update(entt::registry &registry, Renderer &renderer)
{
	auto view = registry.view<Transform, Camera2D>();
	for (auto entity : view)
	{
		auto [transform, camera] = view.get<Transform, Camera2D>(entity);
		if (camera.isMain)
		{
			// Обновляем позицию камеры на основе transform
			Camera2D adjustedCamera = camera;

			RenderParams params;
			params.Position = transform.Position;
			params.Scale = transform.Scale;
			params.Rotation = transform.Rotation;
			params.Origin = transform.Origin;

			// adjustedCamera.offset = transform.Position;
			renderer.SetCamera(adjustedCamera, params);
			return;
		}
	}

	// Камера по умолчанию
	static Camera2D defaultCamera;
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

	// Получаем все объекты со Sprite и Transform
	auto view = registry.view<Transform, Sprite>();

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
			  { return registry.get<Sprite>(lhs).OrderLayer < registry.get<Sprite>(rhs).OrderLayer; });

	// Рендерим в отсортированном порядке
	for (auto entity : visibleEntities)
	{
		const auto &transform = registry.get<Transform>(entity);
		const auto &sprite = registry.get<Sprite>(entity);

		RenderParams params;
		params.Position = transform.Position;
		params.Scale = transform.Scale;
		params.Rotation = transform.Rotation;
		params.Origin = transform.Origin;

		if (sprite.Sprite)
		{
			renderer.RenderSprite(*sprite.Sprite, params);
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
