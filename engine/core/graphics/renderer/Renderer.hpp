#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <engine/core/graphics/textures/Texture2D.hpp>
#include <engine/core/graphics/shaders/Shader.hpp>
#include <engine/core/ecs/components/Transform.hpp>
#include <engine/core/ecs/components/Camera.hpp>

class SpatialPartitioning;

class Renderer
{
public:
	static Renderer &Get();

	// Инициализация — загружаем рендер
	void Init();

	struct RenderParams
	{
		glm::vec2 Position{10.0f, 10.0f}; //
		glm::vec2 Scale{50.0f, 50.0f};	  //

		glm::vec2 SpriteOffset{0.0f, 0.0f};		 //
		glm::vec2 SpriteSize{0.0f, 0.0f};		 //
		glm::vec2 Origin{0, 0};					 //
		float Rotation = 0.0f;					 //
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f}; //
		bool FlipX = false;						 //
		bool FlipY = false;						 //
	};

	bool IsVisible(const Transform &transform);

	Renderer();
	~Renderer();

	void Render(const Texture2D &texture, const RenderParams &params);

	void DrawDebugGrid(const SpatialPartitioning &grid, const glm::vec4 &color);
	void DrawRectOutline(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);

	void SetViewportSize(int width, int height);

	void BeginBatch();
	void EndBatch();

	// ! Камера
	void SetCamera(const Camera &camera, const Transform &transform);
	const glm::mat4 &GetViewMatrix() const { return m_view; }
	const glm::mat4 &GetProjectionMatrix() const { return m_projection; }
	glm::mat4 GetViewProjectionMatrix() const { return m_projection * m_view; }

private:
	// ! Используем фиксированное "логическое" разрешение
	const float LOGICAL_WIDTH = 1280.0f;
	const float LOGICAL_HEIGHT = 720.0f;

	GLuint m_VAO, m_VBO, m_EBO;

	glm::mat4 m_view = glm::mat4(1.0f);
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
