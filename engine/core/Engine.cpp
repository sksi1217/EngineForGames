#include <engine/core/Engine.hpp>
#include <random>
#include <thread>
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
	std::uniform_real_distribution<float> distX(0.0f, 800.0f);
	std::uniform_real_distribution<float> distY(0.0f, 600.0f);

	auto &registry = ECS::Get().GetRegistry();

	GameObject cameraObj = GameObject::Create(registry);

	auto &transformCam = cameraObj.GetComponent<Transform>();
	transformCam.Position = {0, 0};

	cameraObj.SetName("Main Camera");
	auto &camera = cameraObj.AddComponent<Camera>();
	camera.isMain = true;
	cameraObj.AddScript<CameraController>();

	for (int i = 0; i < 1; ++i)
	{

		GameObject entity = GameObject::Create(registry);

		// Генерация случайной позиции
		float x = distX(gen);
		float y = distY(gen);

		// Добавление компонентов
		auto &transform = entity.GetComponent<Transform>();
		transform.Position = {640, 100};

		auto &spriteRender = entity.AddComponent<SpriteRenderer>(texture.get());
		spriteRender.OrderLayer = 1;
		entity.AddScript<ExampleScript>();
		entity.AddScript<Script>();
		spatialPartitioning->AddObject(entity.entity);

		printf("Created object %d with entity id: %u\n", i, static_cast<uint32_t>(entity.entity));
	}
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
	collisionSystem.Update(*spatialPartitioning);
	scriptSystem.Update();
	scriptSystem.FixedUpdate();
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
		ImGuiContext::ShowSettingsWindow(&m_State.showSettingsWindow, m_Window);
	}
	// ! Конец кадра ImGui
	ImGuiContext::EndFrame();
}
