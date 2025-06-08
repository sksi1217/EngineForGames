#include <include/core/Engine.hpp>
#include <random>

Engine::Engine()
{
	Initialize();
}

Engine::~Engine()
{
	utils::Logger::info("Shutting down engine...");
	utils::Logger::shutdown();

	if (m_Window)
	{
		delete m_Window;
		m_Window = nullptr;
	}

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
	m_Window = new GameWindow("My Game Engine", settings.graphics.resolution.x, settings.graphics.resolution.y);
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

	for (int i = 0; i < 100; ++i)
	{
		auto obj = std::make_shared<GameObject>(texture, spatialPartitioning);

		float x = distX(gen); // случайная X
		float y = distY(gen); // случайная Y

		obj->SetPosition(glm::vec2(x, y)); // устанавливаем позицию
		// obj->SetInteractionRange(1);       // Check adjacent cells
		// obj->SetMaxDistance(150.0f);       // Max interaction distance

		gameObj.push_back(obj);
		spatialPartitioning->AddObject(obj);
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
	}
}

void Engine::Update()
{

	// Обновляем объекты по ячейкам
	for (int y = 0; y < spatialPartitioning->GetGridHeight(); ++y)
	{
		for (int x = 0; x < spatialPartitioning->GetGridWidth(); ++x)
		{
			const auto &cell = spatialPartitioning->GetCell(x, y);
			for (const auto &obj : cell)
			{
				if (!obj) // Проверяем, что объект существует
				{
					continue;
				}

				glm::vec2 oldPosition = obj->GetPosition();

				obj->Draw();
				obj->Update();

				if (oldPosition != obj->GetPosition())
				{
					spatialPartitioning->UpdateObjectPosition(obj, oldPosition);
				}
			}
		}
	}

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
	spatialPartitioning->DrawDebug();

	// ! Начало кадра ImGui
	ImGuiContext::BeginFrame();
	if (m_State.showDebugUI)
	{
		ImGuiContext::RenderDebugUI(m_Window, events, &m_State.showDebugUI, gameObj.empty() ? nullptr : gameObj[0].get());
	}

	if (m_State.showSettingsWindow)
	{
		ImGuiContext::ShowSettingsWindow(&m_State.showSettingsWindow, m_Window);
	}
	// ! Конец кадра ImGui
	ImGuiContext::EndFrame();
}
