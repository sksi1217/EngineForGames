#include <engine/core/Systems.hpp>

namespace le
{

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

		/*
		if (transform.GetChanged())
		{
			spatialPartitioning.UpdateObjectPosition(entity, transform.Position);
			transform.SetChanged(false); // ! Сбрасываем флаг
		}*/ });
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

	void DestroySystem::Update()
	{
		auto &registry = ECS::Get().GetRegistry();
		const float dt = utils::Time::DeltaTime();

		// Получаем все сущности с DestroyTimer
		auto view = registry.view<DestroyTimer>();

		// Используем raw-итератор, чтобы можно было удалять сущности безопасно
		for (auto entity : view)
		{
			auto &timer = registry.get<DestroyTimer>(entity);
			timer.timeLeft -= dt;

			if (timer.timeLeft <= 0.0f)
			{
				// Создаём временный Object для корректного уничтожения
				Object obj(registry, entity);
				Destroy(obj); // вызовет OnDestroy и удалит сущность
			}
		}
	}

	// ! Физика
	PhysicsSystem::PhysicsSystem(float worldWidth, float worldHeight, float cellSize)
		: m_worldWidth(worldWidth), m_worldHeight(worldHeight), m_grid(cellSize)
	{
	}

	void PhysicsSystem::Update(entt::registry &registry, float dt)
	{
		if (dt <= 0.0f)
			return;

		IntegratePositions(registry, dt);
		ResolveWorldBounds(registry);
		ResolveCollisions(registry, dt);
	}

	void PhysicsSystem::IntegratePositions(entt::registry &registry, float dt)
	{
		auto view = registry.view<Transform, Rigidbody2D>();
		for (auto entity : view)
		{
			auto &transform = view.get<Transform>(entity);
			auto &rb = view.get<Rigidbody2D>(entity);

			if (rb.isKinematic || rb.isStatic)
				continue;

			// Обновляем скорость: v = v + a * dt
			rb.velocity += rb.acceleration * dt;

			// Обновляем позицию: p = p + v * dt
			transform.Position += rb.velocity * dt;

			// Сбрасываем ускорение (обычно силы прикладываются каждый кадр)
			rb.acceleration = glm::vec2(0.0f);
		}
	}

	void PhysicsSystem::ResolveWorldBounds(entt::registry &registry)
	{
		auto view = registry.view<Transform, Rigidbody2D, BoxCollider2D>();
		for (auto entity : view)
		{
			auto &transform = view.get<Transform>(entity);
			auto &rb = view.get<Rigidbody2D>(entity);
			auto &collider = view.get<BoxCollider2D>(entity);

			if (rb.isKinematic || rb.isStatic)
				continue;

			glm::vec2 worldPos = transform.Position + collider.offset;
			glm::vec2 halfSize = collider.size * 0.5f;

			// Левая граница
			if (worldPos.x - halfSize.x < 0.0f)
			{
				transform.Position.x() = halfSize.x - collider.offset.x;
				rb.velocity.x = glm::abs(rb.velocity.x) * rb.restitution;
			}
			// Правая граница
			if (worldPos.x + halfSize.x > m_worldWidth)
			{
				transform.Position.x() = m_worldWidth - halfSize.x - collider.offset.x;
				rb.velocity.x = -glm::abs(rb.velocity.x) * rb.restitution;
			}
			// Верхняя граница
			if (worldPos.y - halfSize.y < 0.0f)
			{
				transform.Position.y() = halfSize.y - collider.offset.y;
				rb.velocity.y = glm::abs(rb.velocity.y) * rb.restitution;
			}
			// Нижняя граница
			if (worldPos.y + halfSize.y > m_worldHeight)
			{
				transform.Position.y() = m_worldHeight - halfSize.y - collider.offset.y;
				rb.velocity.y = -glm::abs(rb.velocity.y) * rb.restitution;
			}
		}
	}

	void PhysicsSystem::ResolveCollisions(entt::registry &registry, float dt)
	{
		auto view = registry.view<Transform, Rigidbody2D, BoxCollider2D>();

		// Сразу собираем данные и считаем количество
		std::vector<entt::entity> entities;
		std::vector<glm::vec2> positions;
		std::vector<glm::vec2> velocities;
		std::vector<float> invMasses;
		std::vector<glm::vec2> halfSizes;
		std::vector<float> restitutions; // упругость
		std::vector<float> frictions;	 // трение
		std::vector<glm::vec2> offsets;

		for (auto entity : view)
		{
			auto &t = view.get<Transform>(entity);
			auto &rb = view.get<Rigidbody2D>(entity);
			auto &c = view.get<BoxCollider2D>(entity);

			entities.push_back(entity);
			positions.push_back(t.Position);
			velocities.push_back(rb.velocity);
			invMasses.push_back(rb.inverseMass);
			halfSizes.push_back(c.size * 0.5f);
			restitutions.push_back(rb.restitution);
			frictions.push_back(rb.friction);
			offsets.push_back(c.offset);
		}

		size_t totalEntities = entities.size();
		if (totalEntities == 0)
			return;

		// Обновляем grid
		m_grid.ClearGrid();
		for (size_t i = 0; i < totalEntities; ++i)
		{
			glm::vec2 worldPos = positions[i] + offsets[i];
			m_grid.InsertGrid(i, worldPos);
		}

		// === Собираем все контакты один раз ===
		struct CollisionPair
		{
			size_t i, j;
			Contact contact;
			SweptResult swept;
			bool useSwept = false;
			float restitution, friction;
		};
		std::vector<CollisionPair> pairs;

		for (size_t i = 0; i < totalEntities; ++i)
		{
			glm::vec2 worldPosA = positions[i] + offsets[i];
			Aabb aabbA{worldPosA, halfSizes[i]};

			m_grid.ForEachNear(worldPosA, [&](size_t j)
							   {
			if (i >= j) return;

			glm::vec2 worldPosB = positions[j] + offsets[j];
			Aabb aabbB{worldPosB, halfSizes[j]};

			// Сначала пробуем обычную коллизию
			Contact contact = collide(aabbA, aabbB);
			if (contact.intersecting)
			{
				pairs.push_back({i, j, contact, {}, false,
					std::min(restitutions[i], restitutions[j]),
					std::sqrt(frictions[i] * frictions[j])});
				return;
			}

			// Если нет — проверяем swept, если хотя бы один динамический
			bool aDynamic = invMasses[i] > 0.0f;
			bool bDynamic = invMasses[j] > 0.0f;

			if (!aDynamic && !bDynamic) return;

			// Для swept нужна скорость ОДНОГО тела относительно другого
			glm::vec2 relVel = velocities[i] - velocities[j];
			float speed = glm::length(relVel);
			float minSize = std::min({
				halfSizes[i].x * 2, halfSizes[i].y * 2,
				halfSizes[j].x * 2, halfSizes[j].y * 2
			});

			// Эвристика: если за кадр проходит > половины минимального размера — используем swept
			if (speed * dt > minSize * 0.5f)
			{
				SweptResult swept;
				if (aDynamic && !bDynamic)
				{
					// A движется, B статический
					swept = sweptAABB(aabbA, velocities[i], aabbB, dt);
				}
				else if (bDynamic && !aDynamic)
				{
					// B движется, A статический
					swept = sweptAABB(aabbB, velocities[j], aabbA, dt);
					if (swept.hit) swept.normal = -swept.normal; // инвертируем нормаль
				}
				else
				{
					// Оба динамические — упрощённо: считаем B неподвижным, A движется с relVel
					swept = sweptAABB(aabbA, relVel, aabbB, dt);
				}

				if (swept.hit)
				{
					// Создаём Contact из swept
					Contact sweptContact;
					sweptContact.intersecting = true;
					sweptContact.normal = swept.normal;
					// Проникновение — приблизительно
					sweptContact.penetration = 0.01f; // или вычисли точнее

					pairs.push_back({i, j, sweptContact, swept, true,
						std::min(restitutions[i], restitutions[j]),
						std::sqrt(frictions[i] * frictions[j])});
				}
			} });
		}

		// === Velocity & Position решатели (без изменений) ===
		const int velocityIterations = 12;
		for (int iter = 0; iter < velocityIterations; ++iter)
		{
			for (const auto &pair : pairs)
			{
				resolveVelocity(
					velocities[pair.i], velocities[pair.j],
					invMasses[pair.i], invMasses[pair.j],
					pair.contact.normal,
					pair.contact.penetration,
					pair.restitution,
					pair.friction,
					dt);
			}
		}

		const int positionIterations = 4;
		for (int iter = 0; iter < positionIterations; ++iter)
		{
			for (const auto &pair : pairs)
			{
				resolvePosition(
					positions[pair.i], positions[pair.j],
					invMasses[pair.i], invMasses[pair.j],
					pair.contact.normal,
					pair.contact.penetration,
					glm::vec2(0) // точка не критична для swept
				);
			}
		}

		for (auto entity : view)
		{
			auto &transform = view.get<Transform>(entity);
			auto &rb = view.get<Rigidbody2D>(entity);

			if (rb.isKinematic || rb.isStatic)
				continue;

			// Обновляем скорость: v = v + a * dt
			rb.velocity += rb.acceleration * dt;

			// Обновляем позицию: p = p + v * dt
			transform.Position += rb.velocity * dt;

			// Сбрасываем ускорение (обычно силы прикладываются каждый кадр)
			rb.acceleration = glm::vec2(0.0f);
		}

		// Записываем обратно
		for (size_t i = 0; i < totalEntities; ++i)
		{
			registry.get<Transform>(entities[i]).Position = positions[i];
			registry.get<Rigidbody2D>(entities[i]).velocity = velocities[i];
		}
	}
}
