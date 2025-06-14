#pragma once
#include <engine/core/scene/GameObject.hpp>
#include <engine/core/graphics/renderer/Renderer.hpp>
#include <extern/entt/entity/fwd.hpp>

// #include <include/components/Components.hpp>

#include <engine/core/ecs/ECS.hpp>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

class SpatialPartitioning
{
public:
	// ! Конструктор с настройкой размера ячейки сетки
	SpatialPartitioning(int screenWidth, int screenHeight);

	// ! Добавление/удаление объекта
	void AddObject(entt::entity entity);

	const int GetGridWidth() const { return m_GridWidth; }
	const int GetGridHeight() const { return m_GridHeight; }

	int GetCellSize() const { return m_СellSize; }

	const std::vector<entt::entity> &GetCell(int x, int y) const;

	const std::vector<std::vector<std::vector<entt::entity>>> &GetGrid() const { return m_Grid; }

	void DrawDebug();

	void UpdateObjectPosition(entt::entity entity, const glm::vec2 &oldPosition);

	// ? Возвращает список объектов, находящихся в ячейке по координатам позиции или в соседних ячейках
	std::vector<entt::entity> GetNearbyObjects(const glm::vec2 &position) const;

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
