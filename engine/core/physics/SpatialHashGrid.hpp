// SpatialHashGrid.hpp (обновлённая версия с отладкой)
#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>

class SpatialHashGrid
{
public:
	struct Cell
	{
		std::vector<size_t> entities;
	};

	SpatialHashGrid(float cellSize) : cellSize(cellSize) {}

	void ClearGrid() { m_grid.clear(); }

	size_t Hash(const glm::vec2 &pos) const
	{
		int x = static_cast<int>(std::floor(pos.x / cellSize)); // 53 / 100 = 0.02 = 0
		int y = static_cast<int>(std::floor(pos.y / cellSize)); // 47 / 100 = 0.02 = 0

		return (static_cast<size_t>(x) << 16) ^ static_cast<size_t>(y); // объединяем x и y в одно число
	}

	void InsertGrid(size_t entityIndex, const glm::vec2 &pos)
	{
		size_t key = Hash(pos);
		m_grid[key].entities.push_back(entityIndex);
	}

	void ForEachNear(const glm::vec2 &pos, const std::function<void(size_t)> &callback) const
	{
		int x = static_cast<int>(std::floor(pos.x / cellSize));
		int y = static_cast<int>(std::floor(pos.y / cellSize));

		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				size_t key = (static_cast<size_t>(x + dx) << 16) ^ static_cast<size_t>(y + dy);
				auto it = m_grid.find(key);
				if (it != m_grid.end())
				{
					for (size_t idx : it->second.entities)
					{
						callback(idx);
					}
				}
			}
		}
	}

	// const std::unordered_map<size_t, Cell> &getGrid() const { return grid; }
	// float getCellSize() const { return cellSize; }

	void printStats() const
	{
		if (m_grid.empty())
		{
			std::cout << "[SpatialHash] Пусто\n";
			return;
		}

		size_t totalEntities = 0;
		size_t maxEntities = 0;
		for (const auto &pair : m_grid)
		{
			size_t count = pair.second.entities.size();
			totalEntities += count;
			if (count > maxEntities)
				maxEntities = count;
		}

		double avg = static_cast<double>(totalEntities) / m_grid.size();

		std::cout << "[SpatialHash] "
				  << "Ячеек всего: " << m_grid.size()
				  << ", Непустых: " << m_grid.size()
				  << ", Объектов: " << totalEntities
				  << ", Среднее: " << avg
				  << ", Макс: " << maxEntities
				  << ", Размер ячейки: " << cellSize << "\n";
	}

	// === Очистка консоли (кроссплатформенная) ===
	static void clearConsole()
	{
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif
	}

	void printGridAsText(const glm::vec2 &cameraPos, float zoom, float winWidth, float winHeight) const
	{
		clearConsole();

		if (m_grid.empty())
		{
			std::cout << "[SpatialHash] Сетка пуста\n";
			return;
		}

		// === 1. Вычисляем видимую область в МИРОВЫХ координатах ===
		float worldLeft = cameraPos.x - winWidth / (2.0f * zoom);
		float worldRight = cameraPos.x + winWidth / (2.0f * zoom);
		float worldTop = cameraPos.y - winHeight / (2.0f * zoom);
		float worldBottom = cameraPos.y + winHeight / (2.0f * zoom);

		// === 2. Находим ячейки, покрывающие эту область ===
		int cellLeft = static_cast<int>(std::floor(worldLeft / cellSize));
		int cellRight = static_cast<int>(std::floor(worldRight / cellSize));
		int cellTop = static_cast<int>(std::floor(worldTop / cellSize));
		int cellBottom = static_cast<int>(std::floor(worldBottom / cellSize));

		// === 3. Ограничиваем размер вывода ===
		const int MAX_COLS = 25;
		const int MAX_ROWS = 25;
		int cols = std::min(cellRight - cellLeft + 1, MAX_COLS);
		int rows = std::min(cellBottom - cellTop + 1, MAX_ROWS);

		if (cols <= 0 || rows <= 0)
		{
			std::cout << "[SpatialHash] Нет видимых ячеек\n";
			return;
		}

		// === 4. Выводим заголовок ===
		std::cout << "\n[SpatialHash Grid View]\n";
		std::cout << "Камера: (" << static_cast<int>(cameraPos.x)
				  << ", " << static_cast<int>(cameraPos.y) << ") | Зум: " << zoom << "\n";
		std::cout << "Видимая область: [" << static_cast<int>(worldLeft) << ", " << static_cast<int>(worldTop)
				  << "] - [" << static_cast<int>(worldRight) << ", " << static_cast<int>(worldBottom) << "]\n";

		// === 5. Рисуем сетку ===
		std::string topLine(cols * 4 + 1, '_');
		std::cout << topLine << "\n";

		for (int r = 0; r < rows; ++r)
		{
			std::cout << "|";
			for (int c = 0; c < cols; ++c)
			{
				int cellX = cellLeft + c;
				int cellY = cellTop + r;
				size_t key = (static_cast<size_t>(cellX) << 16) ^ static_cast<size_t>(cellY);

				int count = 0;
				auto it = m_grid.find(key);
				if (it != m_grid.end())
				{
					count = static_cast<int>(it->second.entities.size());
				}

				if (count == 0)
				{
					std::cout << "   |";
				}
				else if (count < 10)
				{
					std::cout << " " << count << " |";
				}
				else
				{
					std::cout << count << " |";
				}
			}
			std::cout << "\n";
		}

		std::cout << topLine << "\n";
		std::cout << "Легенда: 0=пусто, 1-2=мало, 3-5=средне, 6-9=много, 10+=перегруз\n";
	}

private:
	float cellSize;
	std::unordered_map<size_t, Cell> m_grid;
};
