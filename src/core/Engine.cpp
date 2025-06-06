#include <include/core/Engine.hpp>

Engine::Engine()
{
	Initialize();

	SetupCallbacks();
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

	// ! Инициализация окна
	m_Window = new GameWindow("My Game Engine", 1280, 720);
	m_Window->SetVSync(true);
	m_Window->SetIcon("icon.png");

	// ! Инициализация GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		utils::Logger::error("Failed to initialize GLEW");
		throw std::runtime_error("Failed to initialize GLEW");
	}

	// Инициализация OpenGL контекста
	glContext.Init(m_Window->GetWindow());

	// Инициализация ImGui
	ImGuiContext::Init(m_Window->GetWindow());

	// ? ShaderManager::Init();

	// Загружаем текстуры
	auto texture1 = TextureLoader::loadTexture("assets/textures/texture.png");
	// Загружаем шейдеров
	auto baseShader = ShaderLoader::loadShader("assets/shaders/vertex.glsl", "assets/shaders/fragment.frag");

	if (!baseShader || !texture1)
	{
		std::cout << "Failed to load Shaders or textures!" << std::endl;
		return;
	}

	auto renderer = std::make_shared<Renderer>();

	auto obj1 = std::make_shared<GameObject>(texture1, baseShader, renderer);

	obj1->SetPosition({100, 100});
	obj1->SetTextureSize({32, 32});

	// obj1->SetTextureRegion({0, 0}, {0, 0});

	gameObj.push_back(obj1);
}

void Engine::Run()
{
	while (!m_Window->ShouldClose() && m_Running)
	{
		// Очистка экрана
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
	for (const auto &obj : gameObj)
	{
		obj->Update();
	}
}

void Engine::Draw()
{

	for (const auto &obj : gameObj)
	{
		obj->Draw();
	}

	if (m_ShowDebugUI)
	{
		ImGuiContext::RenderDebugUI(m_Window, events, &m_ShowDebugUI, gameObj.empty() ? nullptr : gameObj[0].get());
	}
}

void Engine::SetupCallbacks()
{
#pragma region CallBacks
	// Клавиатура
	m_Window->AddKeyCallback(
		[this](int key, int scancode, int action, int mods)
		{
			// Сохраняем информацию о событии
			events.m_LastKeyEvent = {key, scancode, action, mods, static_cast<float>(glfwGetTime())};

			// Передаем события клавиатуры в ImGui
			ImGui_ImplGlfw_KeyCallback(m_Window->GetWindow(), key, scancode, action, mods);

			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				m_Running = false;
			}

			if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			{
				m_ShowDebugUI = !m_ShowDebugUI;
			}
		});

	// Клик мыши
	m_Window->AddMouseButtonCallback(
		[this](int button, int action, int mods)
		{
			// Сохраняем информацию о событии
			events.m_LastMouseButtonEvent = {
				button,
				action,
				mods,
				static_cast<float>(glfwGetTime())};

			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				double x, y;
				glfwGetCursorPos(m_Window->GetWindow(), &x, &y);

				if (action == GLFW_PRESS)
				{
					// Начинаем перетаскивание
					for (auto &obj : gameObj)
					{
						obj->OnMouseClick(static_cast<float>(x), static_cast<float>(y), true);
					}
				}
				else if (action == GLFW_RELEASE)
				{
					// Завершаем перетаскивание
					for (auto &obj : gameObj)
					{
						obj->OnMouseClick(static_cast<float>(x), static_cast<float>(y), false);
					}
				}
			}

			// Передаем события мыши в ImGui
			ImGuiIO &io = ImGui::GetIO();
			if (button >= 0 && button < ImGuiMouseButton_COUNT)
				io.MouseDown[button] = (action == GLFW_PRESS);
		});

	// Перемещение мыши
	m_Window->AddMousePosCallback(
		[this](double x, double y)
		{
			// Сохраняем позицию мыши
			events.m_LastMousePos = {x, y};
			events.m_LastMouseMoveTime = static_cast<float>(glfwGetTime());

			// Передаём координаты объектам для перетаскивания
			for (auto &obj : gameObj)
			{
				obj->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
			}

			// Передаем позицию мыши в ImGui
			ImGuiIO &io = ImGui::GetIO();
			io.MousePos = ImVec2((float)x, (float)y);
		});

	// Прокрутка
	m_Window->AddMouseScrollCallback(
		[this](double x, double y)
		{
			// Сохраняем информацию о прокрутке
			events.m_LastScrollEvent = {
				x,
				y,
				static_cast<float>(glfwGetTime())};

			// Передаем прокрутку в ImGui
			ImGuiIO &io = ImGui::GetIO();
			io.MouseWheelH += (float)x;
			io.MouseWheel += (float)y;
		});

	// Ввод текста
	m_Window->AddCharCallback(
		[this](unsigned int c)
		{
			// Сохраняем введённый символ
			events.m_LastCharInput = {
				c,
				static_cast<float>(glfwGetTime())};

			ImGui_ImplGlfw_CharCallback(m_Window->GetWindow(), c);
		});

	// Изменение размера окна
	m_Window->AddWindowSizeCallback(
		[this](int w, int h)
		{
			events.m_LastWindowSize = {w, h};
			events.m_LastWindowResizeTime = static_cast<float>(glfwGetTime());
		});

#pragma endregion
}
