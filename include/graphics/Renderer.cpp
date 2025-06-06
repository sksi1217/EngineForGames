#include "include/graphics/Renderer.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Конструктор: инициализация буферов и обновление проекции
Renderer::Renderer()
{
	SetupBuffers();
	UpdateProjection();
}

// Деструктор: освобождение ресурсов OpenGL
Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

#pragma region Настройка VBO и EBO для базового квадрата
void Renderer::SetupBuffers()
{
	// Вершины квада: позиция и координаты текстуры
	float vertices[] = {
		// positions        // texture coords
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // нижний левый
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // нижний правый
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // верхний правый
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f  // верхний левый
	};

	unsigned int indices[] = {
		0, 1, 3, // первый треугольник
		1, 2, 3	 // второй треугольник
	};

	// Создаем m_VAO m_VAO m_EBO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	// Активируем VAO
	glBindVertexArray(m_VAO);

	// Привязываем VBO как текущий ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Загружаем данные вершин
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Привязываем EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// Загружаем индексы
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Настраиваем атрибуты вершин: позиция (0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Настраиваем атрибуты вершин: текстурные координаты (1)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Отвязываем VAO
	glBindVertexArray(0);
}
#pragma endregion

#pragma region Обновление проекционной матрицы
void Renderer::UpdateProjection()
{
	// Создаем ортографическую матрицу проекции
	m_projection = glm::ortho(0.0f, static_cast<float>(m_viewportWidth), static_cast<float>(m_viewportHeight), 0.0f, -1.0f, 1.0f);
}
#pragma endregion

#pragma region Установка размера области отрисовки
void Renderer::SetViewportSize(int width, int height)
{
	m_viewportWidth = width;   // Сохраняем ширину
	m_viewportHeight = height; // Сохраняем высоту
	UpdateProjection();		   // Обновляем матрицу проекции
}
#pragma endregion

#pragma region Основной метод отрисовки спрайта
void Renderer::Render(const Texture2D &texture, const Shader &shader, const RenderParams &params)
{
	// Активируем шейдер
	shader.Use();

	// Привязываем текстуру
	texture.Bind();

	// Создаем модельную матрицу
	glm::mat4 model = glm::mat4(1.0f);

	// Перемещение в нужную позицию
	model = glm::translate(model, glm::vec3(params.position, 0.0f));

	// Сдвиг центра объекта к указанной точке опоры (origin)
	model = glm::translate(model, glm::vec3(params.origin * params.size, 0.0f));

	// Поворот вокруг Z-оси
	model = glm::rotate(model, glm::radians(params.rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	// Возвращаем обратно после поворота
	model = glm::translate(model, glm::vec3(-params.origin * params.size, 0.0f));

	// Масштабирование по размеру
	model = glm::scale(model, glm::vec3(params.size, 1.0f));

	// Рассчитываем координаты текстуры
	glm::vec2 texSize = params.textureSize;
	if (texSize.x <= 0 || texSize.y <= 0)
	{
		texSize = glm::vec2(texture.width, texture.height); // По умолчанию — вся текстура
	}

	// Нормализуем координаты текстуры
	glm::vec2 texCoordStart = params.textureOffset / glm::vec2(texture.width, texture.height);
	glm::vec2 texCoordEnd = (params.textureOffset + texSize) / glm::vec2(texture.width, texture.height);

	// Если нужно — отзеркаливаем координаты текстуры
	if (params.flipX)
	{
		std::swap(texCoordStart.x, texCoordEnd.x);
	}
	if (params.flipY)
	{
		std::swap(texCoordStart.y, texCoordEnd.y);
	}

	// Передаем данные в шейдер
	shader.setMat4("model", model);
	shader.setMat4("projection", m_projection);
	shader.setVec4("spriteColor", params.color);
	shader.setVec2("texCoordStart", texCoordStart);
	shader.setVec2("texCoordEnd", texCoordEnd);

	// Выполняем отрисовку
	glBindVertexArray(m_VAO);

	// Рисуем 2 треугольника из 6 индексов
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Отвязываем VAO
	glBindVertexArray(0);
}
#pragma endregion
