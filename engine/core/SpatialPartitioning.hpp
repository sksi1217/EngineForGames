#pragma once

#include <engine/engineapi.hpp>

class SpatialPartitioning
{
public:
	// ! Конструктор с настройкой размера ячейки сетки
	SpatialPartitioning(int screenWidth, int screenHeight);

	// ! Добавление/удаление объекта
	void AddObject(entt::entity entity);

	void DrawDebug();

	void UpdateObjectPosition(entt::entity entity, const glm::vec2 &oldPosition);

	// ! Geters
	const int GetGridWidth() { return m_GridWidth; }
	const int GetGridHeight() { return m_GridHeight; }
	int GetCellSize() { return m_СellSize; }
	const std::vector<entt::entity> &GetCell(int x, int y);
	const std::vector<std::vector<std::vector<entt::entity>>> &GetGrid() { return m_Grid; }
	// ? Возвращает список объектов, находящихся в ячейке по координатам позиции или в соседних ячейках
	std::vector<entt::entity> GetNearbyObjects(const glm::vec2 &position);

private:
	struct GridPositionComponent
	{
		int gridX, gridY;
	};

	// ! Размер ячейки по умолчанию
	int m_СellSize = 200;		   // ! Размер ячейки
	int m_GridWidth, m_GridHeight; // ! Количество клеток по ширине/высоте

	// ! Сетка: каждая ячейка содержит список объектов
	std::vector<std::vector<std::vector<entt::entity>>> m_Grid;

	// ! Структура для хранения позиции в сетке
	struct GridPos
	{
		int x, y;
	};

	int GetObjectCountInCell(int x, int y) const;

	// ! Вспомогательные методы

	GridPos PositionToGridPos(const glm::vec2 &position) const;
};
