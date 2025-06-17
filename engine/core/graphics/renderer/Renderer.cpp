#include <engine/core/graphics/renderer/Renderer.hpp>

Renderer &Renderer::Get()
{
	static Renderer instance;
	return instance;
}

bool Renderer::IsVisible(const Transform &transform)
{
	// Получаем view-projection матрицу
	glm::mat4 viewProjectionMatrix = GetViewProjectionMatrix();

	// Преобразуем координаты объекта (учитываем позицию и масштаб)
	// Создаем vec4, явно указывая все компоненты
	glm::vec4 clipPos = viewProjectionMatrix * glm::vec4(transform.Position.x(), transform.Position.y(), 0.0f, 1.0f);

	// Нормализуем координаты
	glm::vec2 ndc = glm::vec2(clipPos.x, clipPos.y) / clipPos.w;

	// Проверяем, находится ли объект в пределах видимой области
	return (ndc.x >= -1.0f && ndc.x <= 1.0f &&
			ndc.y >= -1.0f && ndc.y <= 1.0f);
}

// Конструктор: инициализация буферов и обновление проекции
Renderer::Renderer()
{
	ShaderManager::Get().Init();

	SetupBuffers();
	UpdateProjection();
}

// Деструктор: освобождение ресурсов OpenGL
Renderer::~Renderer()
{
	utils::Logger::info("Freeing up OpenGL resources!");

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

#pragma region Настройка VBO и EBO для базового квадрата
void Renderer::SetupBuffers()
{
	// ! Вершины квадрата: позиция и координаты текстуры
	float vertices[] = {
		// positions          // texture coords
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // верхний левый
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,  // верхний правый
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // нижний правый
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f // нижний левый
	};

	unsigned int indices[] = {
		0, 1, 3, // ! первый треугольник
		1, 2, 3	 // ! второй треугольник
	};

	// ! Создаем m_VAO m_VAO m_EBO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	// ! Активируем VAO
	glBindVertexArray(m_VAO);

	// ! Привязываем VBO как текущий ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// ! Загружаем данные вершин
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
	m_projection = glm::ortho(0.0f, LOGICAL_WIDTH, LOGICAL_HEIGHT, 0.0f, -1.0f, 1.0f);
}
#pragma endregion

#pragma region Установка размера области отрисовки
void Renderer::SetViewportSize(int width, int height)
{
	// ! Вычисляем желаемый размер viewport'а с сохранением соотношения сторон
	float targetAspect = m_aspectRatio;
	float windowAspect = (float)width / (float)height;

	if (windowAspect > targetAspect)
	{
		// Окно шире, чем целевое соотношение - добавляем черные полосы по бокам
		m_viewportSize.y = height;
		m_viewportSize.x = height * targetAspect;
	}
	else
	{
		// Окно уже - добавляем черные полосы сверху и снизу
		m_viewportSize.x = width;
		m_viewportSize.y = width / targetAspect;
	}

	// Обновляем проекцию с учетом нового размера
	UpdateProjection();

	// Устанавливаем viewport с отступами
	int viewportX = (width - m_viewportSize.x) / 2;
	int viewportY = (height - m_viewportSize.y) / 2;
	glViewport(viewportX, viewportY, m_viewportSize.x, m_viewportSize.y);
}
#pragma endregion

#pragma region Основной метод отрисовки спрайта
void Renderer::RenderSprite(const Texture2D &texture, const RenderParams &params)
{
	m_BatchQueue.emplace_back(&texture, params);
}

#pragma endregion

#pragma region Batch
void Renderer::BeginBatch()
{
	m_BatchQueue.clear(); // Очищаем предыдущие команды
}

void Renderer::EndBatch()
{
	if (m_BatchQueue.empty())
	{
		return;
	}

	auto shader = ShaderManager::Get().GetDefaultShader();
	if (!shader)
	{
		return;
	}

	// ! Активируем шейдер
	shader->Use();

	shader->setMat4("projection", m_projection);
	shader->setMat4("view", m_view);

	glBindVertexArray(m_VAO);

	for (const auto &[texture, params] : m_BatchQueue)
	{
		// ! Проверяем текстуру
		if (!texture)
		{
			return;
		}

		// ! Привязываем текстуру
		texture->Bind();

		// ! Создаем модельную матрицу
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(params.Position, 0.0f));
		model = glm::rotate(model, glm::radians(params.Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(params.Scale, 1.0f));

		const glm::vec2 texSize = (params.SpriteSize.x <= 0 || params.SpriteSize.y <= 0)
									  ? glm::vec2(texture->width, texture->height)
									  : params.SpriteSize;

		// ! Нормализуем координаты текстуры
		const glm::vec2 texInvDims(1.0f / texture->width, 1.0f / texture->height);
		glm::vec2 texCoordStart = params.SpriteOffset * texInvDims;
		glm::vec2 texCoordEnd = (params.SpriteOffset + texSize) * texInvDims;

		// ! Отзеркаливаем координаты текстуры
		if (params.FlipX)
		{
			std::swap(texCoordStart.x, texCoordEnd.x);
		}
		if (params.FlipY)
		{
			std::swap(texCoordStart.y, texCoordEnd.y);
		}

		// ! Передаем данные в шейдер
		shader->setMat4("model", model);

		shader->setVec4("spriteColor", params.Color);
		shader->setVec2("origin", params.Origin);
		shader->setVec2("texCoordStart", texCoordStart);
		shader->setVec2("texCoordEnd", texCoordEnd);

		// ! Выполняем отрисовку
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	// ! Отвязываем VAO
	glBindVertexArray(0);
}

#pragma endregion

#pragma region Рисования

void Renderer::DrawDebugGrid(SpatialPartitioning &grid, const glm::vec4 &color)
{
	int cellSize = grid.GetCellSize();
	int gridWidth = grid.GetGridWidth();
	int gridHeight = grid.GetGridHeight();

	for (int x = 0; x < gridWidth; ++x)
	{
		for (int y = 0; y < gridHeight; ++y)
		{
			// Позиция ячейки
			glm::vec2 position(x * cellSize, y * cellSize);
			glm::vec2 size(cellSize, cellSize);

			// Вызываем уже написанный метод для одной ячейки
			DrawRectOutline(position, size, color);
		}
	}
}

void Renderer::DrawRectOutline(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
{
	auto shader = ShaderManager::Get().GetDefaultShader();
	if (!shader)
		return;

	// Создаем временный VBO и VAO для линий
	GLuint lineVAO, lineVBO;
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);

	// Координаты вершин контура прямоугольника (без текстурных координат)
	float vertices[] = {
		position.x, position.y, 0.0f,					// левый нижний
		position.x + size.x, position.y, 0.0f,			// правый нижний
		position.x + size.x, position.y + size.y, 0.0f, // правый верхний
		position.x, position.y + size.y, 0.0f			// левый верхний
	};

	// Индексы для рисования линий (замкнутый контур)
	unsigned int indices[] = {
		0, 1, // нижняя линия
		1, 2, // правая линия
		2, 3, // верхняя линия
		3, 0  // левая линия
	};

	// Настраиваем VAO и VBO
	glBindVertexArray(lineVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint lineEBO;
	glGenBuffers(1, &lineEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Указываем атрибуты вершин (только позиция)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Настраиваем шейдер
	shader->Use();
	shader->setMat4("model", glm::mat4(1.0f));
	shader->setMat4("projection", m_projection);
	shader->setVec4("spriteColor", color);
	shader->setVec2("texCoordStart", glm::vec2(0.0f));
	shader->setVec2("texCoordEnd", glm::vec2(1.0f));

	// Рисуем линии
	glLineWidth(1.0f); // Толщина линии
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

	// Очищаем ресурсы
	glDeleteVertexArrays(1, &lineVAO);
	glDeleteBuffers(1, &lineVBO);
	glDeleteBuffers(1, &lineEBO);
}
#pragma endregion

#pragma region Камера
void Renderer::SetCamera(const Camera2D &camera, const RenderParams &params)
{
	m_view = glm::mat4(1.0f);
	m_view = glm::translate(m_view, glm::vec3(params.Position, 0.0f));
	m_view = glm::rotate(m_view, glm::radians(params.Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	m_view = glm::scale(m_view, glm::vec3(camera.zoom, camera.zoom, 1.0f));
}
#pragma endregion
