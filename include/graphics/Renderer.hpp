#pragma once

#include <glm/glm.hpp>
#include <include/graphics/Texture2D.hpp>
#include <include/graphics/Shader.hpp>

class Renderer
{
public:
	static Renderer &Get();

	// Инициализация — загружаем рендер
	void Init();

	struct RenderParams
	{
		glm::vec2 position{10.0f, 10.0f};		 // Position on screen
		glm::vec2 size{10.0f, 10.0f};			 // Size of the rendered quad
		glm::vec2 textureOffset{0.0f, 0.0f};	 // Top-left corner of texture region
		glm::vec2 textureSize{0.0f, 0.0f};		 // Size of texture region (0 means use whole texture)
		glm::vec2 origin{0.5f, 0.5f};			 // Normalized origin point (0-1)
		float rotation = 0.0f;					 // Rotation in degrees
		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f}; // Color tint
		bool flipX = false;
		bool flipY = false;
	};

	Renderer();
	~Renderer();

	void Render(const Texture2D &texture, const RenderParams &params);
	void SetViewportSize(int width, int height);

	void RenderLine(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color);

	void DrawRectOutline(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);

private:
	GLuint m_VAO, m_VBO, m_EBO;
	glm::mat4 m_projection;

	float m_aspectRatio = 16.0f / 9.0f; // Соотношение сторон по умолчанию
	glm::vec2 m_viewportSize;			// Реальный размер viewport'а с учетом соотношения сторон

	void SetupBuffers();
	void UpdateProjection();
};
