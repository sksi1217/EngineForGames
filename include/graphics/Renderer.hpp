#pragma once

#include <glm/glm.hpp>
#include <vector>
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
		glm::vec2 position{10.0f, 10.0f}; //
		glm::vec2 size{50.0f, 50.0f};	  //

		glm::vec2 textureOffset{0.0f, 0.0f}; //
		glm::vec2 textureSize{0.0f, 0.0f};//
		glm::vec2 origin{0.5f, 0.5f};//
		float rotation = 0.0f;//
		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f}; //
		bool flipX = false;//
		bool flipY = false;//
	};

	Renderer();
	~Renderer();

	void Render(const Texture2D &texture, const RenderParams &params);
	void SetViewportSize(int width, int height);

	void BeginBatch();
	void EndBatch();

private:
	// ! Используем фиксированное "логическое" разрешение
	const float LOGICAL_WIDTH = 1280.0f;
	const float LOGICAL_HEIGHT = 720.0f;

	GLuint m_VAO, m_VBO, m_EBO;

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
