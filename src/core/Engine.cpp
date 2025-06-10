#include <include/core/Engine.hpp>
#include <random>
#include <thread>

Engine::Engine()
{
	Initialize();
}

Engine::~Engine()
{
	utils::Logger::info("Shutting down engine...");
	utils::Logger::shutdown();

	glfwTerminate();
}

void Engine::Initialize()
{
	// ! Инициализация логирования
	utils::Logger::init();
	utils::Logger::info("Application started");

	// ! Загружаем настройки
	Settings::Get().Load();

	// ! Инициализация окна
	auto &settings = Settings::Get();
	m_Window = std::make_unique<GameWindow>("My Game Engine", settings.graphics.resolution.x, settings.graphics.resolution.y);
	m_Window->ApplySettings(settings);

	// ! Инициализация GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		utils::Logger::error("Failed to initialize GLEW");
		throw std::runtime_error("Failed to initialize GLEW");
	}

	// ! Инициализация OpenGL контекста
	glContext.Init(m_Window->GetWindow());

	// ! Инициализация System Event
	events.Init(m_Window->GetWindow(), &m_State);

	// ! Инициализация ImGui
	ImGuiContext::Init(m_Window->GetWindow());

	spatialPartitioning = new SpatialPartitioning(2000, 1000);

	// ? Временное решение
	// ! Загружаем текстуры
	auto texture = TextureLoader::loadTexture("assets/textures/texture.png");

	if (!texture)
	{
		utils::Logger::error("Failed to load Shaders or textures!");
		std::cout << "Failed to load Shaders or textures!" << std::endl;
		return;
	}

	// Инициализация генератора случайных чисел
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distX(0.0f, 1280.0f);
	std::uniform_real_distribution<float> distY(0.0f, 720.0f);

	auto &registry = ECS::Get().GetRegistry();

	/*
	for (int i = 0; i < 10000; ++i)
	{
		// Создание новой сущности
		entt::entity entity = registry.create();

		// Генерация случайной позиции
		float x = distX(gen);
		float y = distY(gen);

		// Добавление компонентов
		auto &transform = registry.emplace<TransformComponent>(entity);
		transform.position = {x, y};

		registry.emplace<SpriteComponent>(entity, texture.get());

		spatialPartitioning->AddObject(entity);
	}
	*/

	for (int i = 0; i < 100; ++i)
	{
		// Создаем пустой объект
		GameObject entity = GameObject::Create(registry);

		// Генерация случайной позиции
		float x = distX(gen);
		float y = distY(gen);

		// Добавление компонентов
		auto &transform = entity.GetComponent<TransformComponent>();
		transform.position = {x, y};

		// Добавляем к нему SpriteComponent
		entity.AddComponent<SpriteComponent>(texture.get());

		spatialPartitioning->AddObject(entity.entity);
	}

	// ? ---
}

void Engine::Run()
{
	while (!m_Window->ShouldClose() && m_State.isRunning)
	{
		// ! Очистка экрана
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_Window->IsFocused())
		{
			utils::Time::Update();

			glfwPollEvents();

			Update();

			Draw();

			m_Window->SwapBuffers();
		}
		else
		{
			// Снижаем нагрузку, когда окно не в фокусе
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

void Engine::Update()
{

	movementSystem.Update(*spatialPartitioning);

	// Обновление через ECS вместо gameObj
	/*
	auto &registry = ECS::Get().GetRegistry();

	auto view = registry.view<TransformComponent>();
	for (auto entity : view)
	{
		auto &transform = view.get<TransformComponent>(entity);
		glm::vec2 oldPos = transform.position;

		// Обновляем позицию
		// transform.position.x += 10.0f * utils::Time::DeltaTime();

		// Обновляем SpatialPartitioning если позиция изменилась
		if (transform.position != oldPos)
		{
			spatialPartitioning->UpdateObjectPosition(entity, oldPos);
		}
	}

	*/
	/*
	for (const auto &obj : gameEntities)
	{
		if (obj)
		{
			obj->Update();
		}
	}
	*/

	/*
	// ! Пример использования состояния
	if (m_State.keys[GLFW_KEY_W])
	{
		std::cout << "нажата W" << std::endl;
	}

	if (m_State.mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
	{
		std::cout << "нажата ЛКМ" << std::endl;
	}
	*/
}

void Engine::Draw()
{
	if (!spatialPartitioning)
		return;

	// spatialPartitioning->DrawDebug();

	Renderer::Get().BeginBatch();

	// Обновление всех систем
	renderSystem.Update();

	Renderer::Get().EndBatch();

	/*
	// ! Обновляем объекты по ячейкам
	for (int y = 0; y < spatialPartitioning->GetGridHeight(); ++y)
	{
		for (int x = 0; x < spatialPartitioning->GetGridWidth(); ++x)
		{
			const auto &cell = spatialPartitioning->GetCell(x, y);

			for (const auto &obj : cell)
			{
				if (obj)
				{
					const glm::vec2 oldPosition = obj->GetPosition();

					obj->Draw();

					if (oldPosition != obj->GetPosition())
					{
						spatialPartitioning->UpdateObjectPosition(obj, oldPosition);
					}
				}
			}
		}
	}
	*/

	// ! Начало кадра ImGui
	ImGuiContext::BeginFrame();
	if (m_State.showDebugUI)
	{
		ImGuiContext::RenderDebugUI(m_Window, events, &m_State.showDebugUI);
	}

	if (m_State.showSettingsWindow)
	{
		ImGuiContext::ShowSettingsWindow(&m_State.showSettingsWindow, m_Window);
	}
	// ! Конец кадра ImGui
	ImGuiContext::EndFrame();
}
