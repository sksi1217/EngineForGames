#pragma once

// #include <engine/engineapi.hpp>

// #include <GL/glx.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <engine/core/SpatialPartitioning.hpp>

#include <engine/core/graphics/shaders/ShaderManager.hpp>
#include <engine/core/ecs/components/CoreComponents.hpp>

#include <engine/core/graphics/shaders/Shader.hpp>
//

struct Texture2D;
struct RenderParams;
struct Camera2D;
class SpatialPartitioning;

class Renderer
{
public:
	static Renderer &Get();

	// Инициализация — загружаем рендер
	// void Init();

	bool IsVisible(const Transform &transform);

	Renderer();
	~Renderer();

	void RenderSprite(const Texture2D &texture, const RenderParams &params);

	void BeginBatch();
	void EndBatch();

	void SetViewportSize(int width, int height);

	// ** Вспомогательные методы
	void DrawDebugGrid(SpatialPartitioning &grid, const glm::vec4 &color);

	void DrawRectOutline(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
	// ** ---

	// ! Камера
	void SetCamera(const Camera2D &camera, const RenderParams &params);
	// const glm::mat4 &GetViewMatrix() const { return m_view; }
	// const glm::mat4 &GetProjectionMatrix() const { return m_projection; }
	glm::mat4 GetViewProjectionMatrix() const { return m_projection * m_view; }

private:
	// ! Используем фиксированное "логическое" разрешение
	const float LOGICAL_WIDTH = 1280.0f;
	const float LOGICAL_HEIGHT = 720.0f;

	GLuint m_VAO, m_VBO, m_EBO;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	const float m_aspectRatio = 16.0f / 9.0f;

	glm::ivec2 m_viewportSize;

	/* // ! пример как выглядит pair
		{
			{2, "baz"},
			{2, "bar"},
			{1, "foo"}
		};
	*/

	std::vector<std::pair<const Texture2D *, RenderParams>> m_BatchQueue;

	void SetupBuffers();
	void UpdateProjection();
};
