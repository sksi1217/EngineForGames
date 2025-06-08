#pragma once
#include <include/core/GameObject.hpp>
#include <include/graphics/Renderer.hpp>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

class SpatialPartitioning
{
public:
	// Конструктор с настройкой размера ячейки сетки
	SpatialPartitioning(int screenWidth, int screenHeight);

	// Добавление/удаление объекта
	void AddObject(std::shared_ptr<GameObject> obj);

	const int GetGridWidth() const { return m_GridWidth; }
	const int GetGridHeight() const { return m_GridHeight; }

	int GetCellSize() const { return m_СellSize; }

	const std::vector<std::shared_ptr<GameObject>> &GetCell(int x, int y) const;

	const std::vector<std::vector<std::vector<std::shared_ptr<GameObject>>>> &GetGrid() const { return m_Grid; }

	void DrawDebug();

	void UpdateObjectPosition(std::shared_ptr<GameObject> obj, const glm::vec2 &oldPosition);

private:
	// Размер ячейки по умолчанию
	int m_СellSize = 200;		   // Размер ячейки
	int m_GridWidth, m_GridHeight; // Количество клеток по ширине/высоте

	// ! Сетка: каждая ячейка содержит список объектов
	std::vector<std::vector<std::vector<std::shared_ptr<GameObject>>>> m_Grid;

	// ? m_Grid[cell.x][cell.y].push_back(object);
	// ? m_grid[cell.x][cell.y].erase(object); // удалить объект

	// Структура для хранения позиции в сетке
	struct GridPos
	{
		int x, y;
	};

	int GetObjectCountInCell(int x, int y) const;

	// Вспомогательные методы
	GridPos PositionToGridPos(const glm::vec2 &position) const;
};
