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

void SpatialPartitioning::AddObject(std::shared_ptr<GameObject> obj)
{
	glm::vec2 positionObj = obj->GetPosition();		  // ! позиция объектва
	GridPos gridPos = PositionToGridPos(positionObj); // ! позиция на сетке

	if (gridPos.x >= 0 && gridPos.x < m_GridWidth &&
		gridPos.y >= 0 && gridPos.y < m_GridHeight)
	{
		m_Grid[gridPos.x][gridPos.y].push_back(obj);
	}
	else
	{
		utils::Logger::error("Object out of grid bounds");
	}
}

const std::vector<std::shared_ptr<GameObject>> &SpatialPartitioning::GetCell(int x, int y) const
{
	static std::vector<std::shared_ptr<GameObject>> empty;

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

void SpatialPartitioning::UpdateObjectPosition(std::shared_ptr<GameObject> obj, const glm::vec2 &oldPosition)
{
	// Get old grid position
	GridPos oldGridPos = PositionToGridPos(oldPosition);

	// Get new grid position
	glm::vec2 newPosition = obj->GetPosition();
	GridPos newGridPos = PositionToGridPos(newPosition);

	// If the object moved to a different cell
	if (oldGridPos.x != newGridPos.x || oldGridPos.y != newGridPos.y)
	{
		// Remove from old cell if it was in bounds
		if (oldGridPos.x >= 0 && oldGridPos.x < m_GridWidth &&
			oldGridPos.y >= 0 && oldGridPos.y < m_GridHeight)
		{
			auto &oldCell = m_Grid[oldGridPos.x][oldGridPos.y];
			oldCell.erase(std::remove(oldCell.begin(), oldCell.end(), obj), oldCell.end());
		}

		// Add to new cell if it's in bounds
		if (newGridPos.x >= 0 && newGridPos.x < m_GridWidth &&
			newGridPos.y >= 0 && newGridPos.y < m_GridHeight)
		{
			m_Grid[newGridPos.x][newGridPos.y].push_back(obj);
		}
	}
}

/*


void SpatialPartitioning::GetGridPositionsInArea(const glm::vec2 &topLeft, const glm::vec2 &bottomRight,
												 std::vector<GridPos> &outPositions) const
{
	GridPos start = PositionToGridPos(topLeft);
	GridPos end = PositionToGridPos(bottomRight);

	for (int y = start.y; y <= end.y; ++y)
	{
		for (int x = start.x; x <= end.x; ++x)
		{
			outPositions.push_back({x, y});
		}
	}
}



void SpatialPartitioning::RemoveObject(std::shared_ptr<GameObject> obj)
{
	if (!obj)
		return;

	GridPos pos = PositionToGridPos(obj->GetPosition());
	auto range = m_grid.equal_range(pos);

	for (auto it = range.first; it != range.second;)
	{
		if (it->second == obj)
		{
			it = m_grid.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}
}

void SpatialPartitioning::UpdateObject(std::shared_ptr<GameObject> obj)
{

	glm::vec2 newPosition = object->GetPosition();

	// Если позиция не изменилась, ничего не делаем
	if (oldPosition == newPosition)
	{
		return;
	}

	// Удаляем объект из старой ячейки
	glm::ivec2 oldCell = GetCellIndices(oldPosition);
	if (IsValidCell(oldCell.x, oldCell.y))
	{
		for (auto it = m_grid[oldCell.x][oldCell.y].begin(); it != m_grid[oldCell.x][oldCell.y].end(); ++it)
		{
			if (it->lock() == object)
			{
				m_grid[oldCell.x][oldCell.y].erase(it);
				break;
			}
		}
	}

	// Добавляем в новую ячейку
	AddObject(object);
}

glm::ivec2 SpatialPartition::GetCellIndices(const glm::vec2 &position) const
{
	int cellX = static_cast<int>(position.x / m_cellSize);
	int cellY = static_cast<int>(position.y / m_cellSize);

	// Убедиться, что мы находимся в пределах допустимых значений
	cellX = std::max(0, std::min(cellX, m_gridWidth - 1));
	cellY = std::max(0, std::min(cellY, m_gridHeight - 1));

	return glm::ivec2(cellX, cellY);
}

std::vector<std::shared_ptr<GameObject>> SpatialPartitioning::Query(const glm::vec2 &position, float radius) const
{
	glm::vec2 topLeft(position.x - radius, position.y - radius);
	glm::vec2 bottomRight(position.x + radius, position.y + radius);
	return Query(topLeft, bottomRight);
}

std::vector<std::shared_ptr<GameObject>> SpatialPartitioning::Query(const glm::vec2 &topLeft, const glm::vec2 &bottomRight) const
{
	std::vector<GridPos> gridPositions;
	GetGridPositionsInArea(topLeft, bottomRight, gridPositions);

	std::vector<std::shared_ptr<GameObject>> result;

	for (const auto &pos : gridPositions)
	{
		auto range = m_grid.equal_range(pos);
		for (auto it = range.first; it != range.second; ++it)
		{
			const auto &obj = it->second;
			const glm::vec2 &objPos = obj->GetPosition();

			// Проверяем, действительно ли объект находится в запрашиваемой области
			if (objPos.x >= topLeft.x && objPos.x <= bottomRight.x &&
				objPos.y >= topLeft.y && objPos.y <= bottomRight.y)
			{
				result.push_back(obj);
			}
		}
	}

	return result;
}

void SpatialPartitioning::Clear()
{
	m_grid.clear();
}
*/
