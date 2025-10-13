#include <engine/core/Engine.hpp>

#include <tests/ExampleScript.hpp>
#include <tests/Script.hpp>
#include <tests/CameraController.hpp>

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
	// Settings::Get().Load();

	// ! Инициализация окна
	// auto &settings = Settings::Get();
	m_Window = std::make_unique<GameWindow>();
	m_Window->SetTitle("My Game Engine");
	// m_Window->SetSize(settings.graphics.resolution.x, settings.graphics.resolution.y);
	// m_Window->ApplySettings(settings);

	// ! Инициализация GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		utils::Logger::error("Failed to initialize GLEW");
		throw std::runtime_error("Failed to initialize GLEW");
	}

	// ! Инициализация OpenGL контекста
	glContext.Init(m_Window->GetWindowGLFW());

	// ! Инициализация System Event
	events.Init(m_Window->GetWindowGLFW(), &m_State);

	// ! Инициализация ImGui
	ImGuiContext::Init(m_Window->GetWindowGLFW());
	// spatialPartitioning = new SpatialPartitioning(2000, 1000);

	auto &registry = ECS::Get().GetRegistry();

	// ! Добавления камеры
	Object camera = Object::CreateObject(registry);
	camera.SetName("Main Camera");
	auto &camera2D = camera.AddComponent<Camera2D>();
	camera2D.isMain = true;
	camera.AddScript<CameraController>();

	// ? Временное решение
	// ! Загружаем текстуры
	auto texture = TextureLoader::loadTexture("assets/textures/texture.png");

	if (!texture)
	{
		utils::Logger::error("Failed to load Shaders or textures!");
		std::cout << "Failed to load Shaders or textures!" << std::endl;
		return;
	}

	Object entity = Object::CreateObject(registry);
	auto &transform = entity.GetComponent<Transform>();
	transform.Position = {100, 100};
	transform.Scale = {100, 100};
	entity.AddComponent<Sprite>(texture.get());
	entity.AddScript<ExampleScript>();
	entity.AddScript<Script>();

	// spatialPartitioning->AddObject(entity.entity, {transform.Position.x(), transform.Position.y()});

	Object entity1 = Object::CreateObject(registry);
	auto &transform1 = entity1.GetComponent<Transform>();
	transform1.Position = {100, 100};
	transform1.Scale = {100, 100};
	entity1.AddComponent<Sprite>(texture.get());

	entity.GetScript<Script>().targetEntity = entity1.entity;

	// spatialPartitioning->AddObject(entity1.entity, {transform1.Position.x(), transform1.Position.y()});
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
			InputManager::Get().Update(m_Window->GetWindowGLFW());

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
	// Сначала обновляем физику и коллизии
	// collisionSystem.Update(*spatialPartitioning);

	// Затем обновляем позиции и проверяем изменения
	movementSystem.Update(*spatialPartitioning);
	DestroySystem::Update();
	// Потом скрипты
	scriptSystem.Update();
	scriptSystem.FixedUpdate();

	// destroySystem.Update();
	// И в конце рендер
	renderSystem.Update();
}

void Engine::Draw()
{
	if (!spatialPartitioning)
		return;

	Renderer::Get().BeginBatch();

	// ! Выводим в консоль сколько объектов в spatialPartitioning
	// spatialPartitioning->DrawDebug();
	// ! Рисуем всю сетку
	// Renderer::Get().DrawDebugGrid(*spatialPartitioning, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));

	// ! Обновление всех систем
	renderSystem.Update();

	Renderer::Get().EndBatch();

	// ** ------------

	// ! Начало кадра ImGui
	ImGuiContext::BeginFrame();

	if (m_State.showDebugUI)
	{
		// ImGuiContext::RenderObjectUI(m_Objects.empty() ? nullptr : m_Objects[0].get());
	}

	if (m_State.showDebugUI)
	{
		ImGuiContext::RenderDebugUI(m_Window, events, &m_State.showDebugUI);
	}

	if (m_State.showSettingsWindow)
	{
		// ImGuiContext::ShowSettingsWindow(&m_State.showSettingsWindow, m_Window);
	}
	// ! Конец кадра ImGui
	ImGuiContext::EndFrame();
}
