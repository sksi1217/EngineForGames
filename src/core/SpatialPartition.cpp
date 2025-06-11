#include <include/core/SpatialPartitioning.hpp>
#include <algorithm>

SpatialPartitioning::SpatialPartitioning(int screenWidth, int screenHeight)
{
	m_СellSize = 100;

	m_GridWidth = screenWidth / m_СellSize + 1;
	m_GridHeight = screenHeight / m_СellSize + 1;

	// создаю клетки по Количеству клеток по ширине/высоте
	m_Grid.resize(m_GridWidth);
	for (int i = 0; i < m_GridWidth; ++i)
	{
		m_Grid[i].resize(m_GridHeight);
	}
}

void SpatialPartitioning::AddObject(entt::entity entity)
{
	auto &registry = ECS::Get().GetRegistry();
	if (!registry.valid(entity))
		return;

	const auto &transform = registry.get<TransformComponent>(entity);
	GridPos gridPos = PositionToGridPos(transform.position);

	registry.emplace_or_replace<GridPositionComponent>(entity, gridPos.x, gridPos.y);

	if (gridPos.x >= 0 && gridPos.x < m_GridWidth &&
		gridPos.y >= 0 && gridPos.y < m_GridHeight)
	{
		m_Grid[gridPos.x][gridPos.y].push_back(entity);
	}
}

const std::vector<entt::entity> &SpatialPartitioning::GetCell(int x, int y) const
{
	static std::vector<entt::entity> empty;

	if (x >= 0 && x < m_GridWidth && y >= 0 && y < m_GridHeight)
	{
		return m_Grid[x][y];
	}

	return empty; // безопасный возврат, если индекс вне диапазона
}

SpatialPartitioning::GridPos SpatialPartitioning::PositionToGridPos(const glm::vec2 &position) const
{
	return {
		static_cast<int>(std::floor(position.x / m_СellSize)),
		static_cast<int>(std::floor(position.y / m_СellSize))};
}

int SpatialPartitioning::GetObjectCountInCell(int x, int y) const
{
	if (x >= 0 && x < m_GridWidth && y >= 0 && y < m_GridHeight)
	{
		return static_cast<int>(m_Grid[x][y].size());
	}
	return 0;
}

void SpatialPartitioning::UpdateObjectPosition(entt::entity entity, const glm::vec2 &oldPosition)
{
	auto &registry = ECS::Get().GetRegistry();
	if (!registry.valid(entity))
		return;

	const auto &transform = registry.get<TransformComponent>(entity);
	auto &gridPos = registry.get<GridPositionComponent>(entity);

	GridPos newGridPos = PositionToGridPos(transform.position);

	if (gridPos.gridX != newGridPos.x || gridPos.gridY != newGridPos.y)
	{
		// Remove from old cell
		if (gridPos.gridX >= 0 && gridPos.gridX < m_GridWidth &&
			gridPos.gridY >= 0 && gridPos.gridY < m_GridHeight)
		{
			auto &oldCell = m_Grid[gridPos.gridX][gridPos.gridY];
			oldCell.erase(std::remove(oldCell.begin(), oldCell.end(), entity), oldCell.end());
		}

		// Update grid position
		gridPos.gridX = newGridPos.x;
		gridPos.gridY = newGridPos.y;

		// Add to new cell
		if (newGridPos.x >= 0 && newGridPos.x < m_GridWidth &&
			newGridPos.y >= 0 && newGridPos.y < m_GridHeight)
		{
			m_Grid[newGridPos.x][newGridPos.y].push_back(entity);
		}
	}
}

std::vector<entt::entity> SpatialPartitioning::GetNearbyObjects(const glm::vec2 &position) const
{
	GridPos pos = PositionToGridPos(position);
	std::vector<entt::entity> result;

	// Проверяем текущую ячейку и все соседние (3x3)
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			int x = pos.x + dx;
			int y = pos.y + dy;

			if (x >= 0 && y >= 0 && x < m_GridWidth && y < m_GridHeight)
			{
				result.insert(result.end(), m_Grid[x][y].begin(), m_Grid[x][y].end());
			}
		}
	}

	return result;
}

void SpatialPartitioning::DrawDebug()
{
	system("clear");

	// Выводим заголовок
	std::cout << "Spatial Partitioning Grid (" << m_GridWidth << "x" << m_GridHeight << "):\n";

	// Верхняя граница таблицы
	std::cout << "+";
	for (int x = 0; x < m_GridWidth; ++x)
	{
		std::cout << "----+";
	}
	std::cout << "\n";

	// Рисуем сетку в консоли (теперь сверху вниз)
	for (int y = 0; y < m_GridHeight; ++y) // Изменили направление цикла
	{
		std::cout << "|";
		for (int x = 0; x < m_GridWidth; ++x)
		{
			// Количество объектов в ячейке
			int count = GetObjectCountInCell(x, y);

			// Выбираем цвет текста в зависимости от количества объектов
			if (count == 0)
			{
				// Серый - пусто (используем стандартный цвет консоли)
				std::cout << "    ";
			}
			else if (count < 3)
			{
				// Зеленый - мало объектов
				std::cout << "\033[32m";
				std::cout << std::setw(3) << count << " ";
				std::cout << "\033[0m";
			}
			else if (count < 6)
			{
				// Желтый - среднее количество
				std::cout << "\033[33m";
				std::cout << std::setw(3) << count << " ";
				std::cout << "\033[0m";
			}
			else
			{
				// Красный - много объектов
				std::cout << "\033[31m";
				std::cout << std::setw(3) << count << " ";
				std::cout << "\033[0m";
			}
			std::cout << "|";
		}
		std::cout << "\n";

		// Горизонтальная линия между строками
		std::cout << "+";
		for (int x = 0; x < m_GridWidth; ++x)
		{
			std::cout << "----+";
		}
		std::cout << "\n";
	}

	// Легенда
	std::cout << "\nLegend:\n";
	std::cout << "\033[32mGreen\033[0m: 1-2 objects\n";
	std::cout << "\033[33mYellow\033[0m: 3-5 objects\n";
	std::cout << "\033[31mRed\033[0m: 6+ objects\n";
	std::cout << "Empty: 0 objects\n\n";
}
