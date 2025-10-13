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
		if (!registry.all_of<ActiveComponent>(entity))
		{
			// Если компонента нет — считаем объект активным (как в Unity)
			// ИЛИ пропускаем, если ваша логика требует обязательного наличия компонента
			// Здесь — считаем активным по умолчанию
			// → не пропускаем, продолжаем рендер
		}
		else
		{
			const auto &active = registry.get<ActiveComponent>(entity);
			if (!active.isActive)
				continue;
		}

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
/*
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
*/

void CollisionSystem::Update(SpatialPartitioning &spatialPartitioning)
{
	auto &registry = ECS::Get().GetRegistry();
	auto view = registry.view<Transform, ActiveComponent, BoxCollider2D>();

	view.each([&](auto entity, const Transform &transform, const ActiveComponent &active, const BoxCollider2D &collider)
			  {
        if (!active.isActive)
            return;

        auto nearbyEntities = spatialPartitioning.GetNearbyObjects(entity);

        for (auto otherEntity : nearbyEntities)
        {
            if (entity == otherEntity)
                continue;

            if (!registry.valid(otherEntity))
                continue;

            if (!registry.all_of<Transform, BoxCollider2D>(otherEntity))
                continue;

            const Transform &otherTransform = registry.get<Transform>(otherEntity);
            const BoxCollider2D &otherCollider = registry.get<BoxCollider2D>(otherEntity);

            if (CheckAABB(collider, transform, otherCollider, otherTransform)) {
                HandleCollision(entity, otherEntity);
            }
        } });
}

bool CollisionSystem::CheckAABB(const BoxCollider2D &a, const Transform &ta,
								const BoxCollider2D &b, const Transform &tb)
{
	glm::vec2 minA = a.GetMin(ta);
	glm::vec2 maxA = a.GetMax(ta);

	glm::vec2 minB = b.GetMin(tb);
	glm::vec2 maxB = b.GetMax(tb);

	return (minA.x < maxB.x && maxA.x > minB.x &&
			minA.y < maxB.y && maxA.y > minB.y);
}

void CollisionSystem::ResolveCollision(entt::registry &registry, entt::entity a, entt::entity b)
{
	if (!registry.all_of<Transform, Rigidbody2D, BoxCollider2D>(a) ||
		!registry.all_of<Transform, Rigidbody2D, BoxCollider2D>(b))
	{
		return;
	}

	auto &transformA = registry.get<Transform>(a);
	auto &rbA = registry.get<Rigidbody2D>(a);
	auto &colliderA = registry.get<BoxCollider2D>(a);

	auto &transformB = registry.get<Transform>(b);
	auto &rbB = registry.get<Rigidbody2D>(b);
	auto &colliderB = registry.get<BoxCollider2D>(b);

	// Получаем AABB
	glm::vec2 minA = colliderA.GetMin(transformA);
	glm::vec2 maxA = colliderA.GetMax(transformA);

	glm::vec2 minB = colliderB.GetMin(transformB);
	glm::vec2 maxB = colliderB.GetMax(transformB);

	// Вычисляем глубину пересечения по X и Y
	float overlapX = std::min(maxA.x - minB.x, maxB.x - minA.x);
	float overlapY = std::min(maxA.y - minB.y, maxB.y - minA.y);

	// Определяем направление нормали
	glm::vec2 normal(0.0f);
	float penetration = 0.0f;

	if (std::abs(overlapX) < std::abs(overlapY))
	{
		penetration = overlapX;
		normal = glm::vec2(penetration < 0 ? -1.0f : 1.0f, 0.0f);
	}
	else
	{
		penetration = overlapY;
		normal = glm::vec2(0.0f, penetration < 0 ? -1.0f : 1.0f);
	}

	const float slop = 0.05f; // Порог для коррекции
	penetration = std::max(std::abs(penetration) - slop, 0.0f);

	float invMassA = rbA.isKinematic ? 0.0f : 1.0f / rbA.mass;
	float invMassB = rbB.isKinematic ? 0.0f : 1.0f / rbB.mass;

	float correction = penetration / (invMassA + invMassB);

	transformA.Position -= invMassA * correction * normal;
	transformB.Position += invMassB * correction * normal;
}

void CollisionSystem::HandleCollision(entt::entity a, entt::entity b)
{
	auto &registry = ECS::Get().GetRegistry();

	// Вызываем OnCollide у скриптов для обеих сущностей
	if (registry.all_of<ScriptsContainerComponent>(a))
	{
		auto &container = registry.get<ScriptsContainerComponent>(a);
		for (auto &script : container.scripts)
		{
			// script->OnCollide(b); // Передаем вторую сущность
		}
	}

	if (registry.all_of<ScriptsContainerComponent>(b))
	{
		auto &container = registry.get<ScriptsContainerComponent>(b);
		for (auto &script : container.scripts)
		{
			// script->OnCollide(a); // Передаем первую сущность
		}
	}

	// Разрешение физики между двумя объектами
	ResolveCollision(registry, a, b);
}
