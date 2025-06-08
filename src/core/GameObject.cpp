#include <include/core/GameObject.hpp>
#include <random>

GameObject::GameObject(std::shared_ptr<Texture2D> texture, SpatialPartitioning *spatial)
	: m_texture(texture), m_spatial(spatial)
{
	if (!texture)
	{
		utils::Logger::error("Texture is null!");
		throw std::runtime_error("Texture is null!");
	}

	// Инициализация случайной скорости
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	m_velocity = glm::vec2(dist(gen), dist(gen));
	m_velocity = glm::normalize(m_velocity) * 100.0f; // Нормализуем и задаем скорость
}

GameObject::~GameObject()
{
}

void GameObject::Draw()
{
	if (m_texture)
	{
		Renderer::Get().Render(*m_texture, m_params);
	}
}

void GameObject::Update()
{
	if (m_params.rotation >= 360)
	{
		m_params.rotation = 0;
	}
	else
	{
		// ? 45 градусов в секунду
		m_params.rotation += 45.0f * utils::Time::DeltaTime();
	}

	// Движение
	m_params.position += m_velocity * utils::Time::DeltaTime();

	// Отражение от границ
	if (m_params.position.x <= 0 || m_params.position.x >= 800)
	{
		m_velocity.x *= -1;
		m_params.position.x = glm::clamp(m_params.position.x, 0.0f, 800.0f);
	}

	if (m_params.position.y <= 0 || m_params.position.y >= 600)
	{
		m_velocity.y *= -1;
		m_params.position.y = glm::clamp(m_params.position.y, 0.0f, 600.0f);
	}

	CheckNearbyObjects();
}

void GameObject::CheckNearbyObjects()
{
	// Get current grid position
	int gridX = static_cast<int>(m_params.position.x / m_spatial->GetCellSize());
	int gridY = static_cast<int>(m_params.position.y / m_spatial->GetCellSize());

	int gridWidth = m_spatial->GetGridWidth();
	int gridHeight = m_spatial->GetGridHeight();

	// Check nearby cells
	for (int dx = -m_interactionRange; dx <= m_interactionRange; dx++)
	{
		for (int dy = -m_interactionRange; dy <= m_interactionRange; dy++)
		{
			// Calculate grid coordinates with wrapping
			int checkX = (gridX + dx + gridWidth) % gridWidth;
			int checkY = (gridY + dy + gridHeight) % gridHeight;

			// Get objects in this cell
			const auto &objects = m_spatial->GetCell(checkX, checkY);

			// Check each object in the cell
			for (const auto &obj : objects)
			{
				if (this != obj.get())
				{ // Skip self
					float d = glm::distance(m_params.position, obj->GetPosition());
					if (d < m_maxDistance)
					{
						// Draw a line between interacting objects
						Renderer::Get().RenderLine(
							m_params.position,
							obj->GetPosition(),
							glm::vec4(1.0f, 1.0f, 1.0f, 0.5f) // White with some transparency
						);

						// Here you can add other interaction logic
					}
				}
			}
		}
	}
}
