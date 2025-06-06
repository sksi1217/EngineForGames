#pragma once

#include <glm/glm.hpp>
#include "src/graphics/Texture2D.hpp"
#include "src/graphics/Shader.hpp"

class Renderer
{
public:
	struct RenderParams
	{
		glm::vec2 position{10.0f, 10.0f};		 // Position on screen
		glm::vec2 size{100.0f, 100.0f};			 // Size of the rendered quad
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

	void Render(const Texture2D &texture, const Shader &shader, const RenderParams &params);
	void SetViewportSize(int width, int height);

private:
	GLuint m_VAO, m_VBO, m_EBO;
	glm::mat4 m_projection;
	int m_viewportWidth = 1280;
	int m_viewportHeight = 720;

	void SetupBuffers();
	void UpdateProjection();
};
