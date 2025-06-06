#include "ImGuiContext.hpp"

void ImGuiContext::Init(GLFWwindow *window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

ImGuiContext::~ImGuiContext()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiContext::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiContext::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiContext::RenderDebugUI(GameWindow *m_Window, EventSystem &events, bool *showUI, GameObject *gameObject)
{
	BeginFrame(); // Начало кадра ImGui

	// Создаём окно с debug информацией
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoCollapse);

	// --- Поле ввода текста ---
	ImGui::Separator();
	ImGui::Text("Input text:");
	if (ImGui::InputText("##input", m_InputText, sizeof(m_InputText), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		std::cout << "Введено: " << m_InputText << std::endl;
	}

	// FPS и время кадра
	ImGui::Text("FPS: %.1f", utils::Time::FPS());
	ImGui::Text("Frame time: %.3f ms", utils::Time::DeltaTime() * 1000.0f);

	// Разделитель
	ImGui::Separator();

	// Информация о GPU
	ImGui::Text("OpenGL:");
	ImGui::Text("  Vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("  Renderer: %s", glGetString(GL_RENDERER));
	ImGui::Text("  Version: %s", glGetString(GL_VERSION));

	// Разделитель
	ImGui::Separator();

	// Информация о системе
	ImGui::Text("Window:");
	int width, height;
	glfwGetWindowSize(m_Window->GetWindow(), &width, &height);
	ImGui::Text("  Size: %d x %d", width, height);
	ImGui::Text("  Last resize: %d x %d (%.2f sec ago)",
				events.m_LastWindowSize.w, events.m_LastWindowSize.h,
				static_cast<float>(glfwGetTime()) - events.m_LastWindowResizeTime);

	// Разделитель
	ImGui::Separator();

	// Информация о событиях
	ImGui::Text("Last Events:");

	// Клавиатура
	ImGui::Text("Keyboard:");
	ImGui::Text("  Key: %d, Action: %s, Mods: %d (%.2f sec ago)",
				events.m_LastKeyEvent.key,
				events.m_LastKeyEvent.action == GLFW_PRESS ? "PRESS" : events.m_LastKeyEvent.action == GLFW_RELEASE ? "RELEASE"
																													: "REPEAT",
				events.m_LastKeyEvent.mods,
				static_cast<float>(glfwGetTime()) - events.m_LastKeyEvent.time);

	// Мышь
	ImGui::Text("Mouse:");
	ImGui::Text("  Position: %.1f, %.1f (%.2f sec ago)",
				events.m_LastMousePos.x, events.m_LastMousePos.y,
				static_cast<float>(glfwGetTime()) - events.m_LastMouseMoveTime);
	ImGui::Text("  Button: %d, Action: %s, Mods: %d (%.2f sec ago)",
				events.m_LastMouseButtonEvent.button,
				events.m_LastMouseButtonEvent.action == GLFW_PRESS ? "PRESS" : "RELEASE",
				events.m_LastMouseButtonEvent.mods,
				static_cast<float>(glfwGetTime()) - events.m_LastMouseButtonEvent.time);
	ImGui::Text("  Scroll: X=%.1f, Y=%.1f (%.2f sec ago)",
				events.m_LastScrollEvent.x, events.m_LastScrollEvent.y,
				static_cast<float>(glfwGetTime()) - events.m_LastScrollEvent.time);

	// Ввод текста
	ImGui::Text("Char Input:");
	ImGui::Text("  Char: %u ('%c') (%.2f sec ago)",
				events.m_LastCharInput.character,
				isprint(events.m_LastCharInput.character) ? events.m_LastCharInput.character : ' ',
				static_cast<float>(glfwGetTime()) - events.m_LastCharInput.time);

	// Разделитель
	ImGui::Separator();

	// Кнопка для закрытия окна
	if (ImGui::Button("Close Debug"))
	{
		*showUI = false;
	}

	if (gameObject)
	{
		RenderObjectUI(gameObject);
	}

	ImGui::End();

	EndFrame(); // Конец кадра ImGui
}

void ImGuiContext::RenderObjectUI(GameObject *gameObject)
{
	if (!gameObject)
		return;

	ImGui::Begin("Object Properties", nullptr, ImGuiWindowFlags_NoCollapse);

	// Позиция
	glm::vec2 pos = gameObject->GetPosition();
	if (ImGui::DragFloat2("Position", &pos.x, 1.0f))
	{
		gameObject->SetPosition(pos);
	}

	// Размер
	glm::vec2 size = gameObject->GetSize();
	if (ImGui::DragFloat2("Size", &size.x, 1.0f, 1.0f, 1000.0f))
	{
		gameObject->SetSize(size);
	}

	// Смещение текстуры
	glm::vec2 texOffset = gameObject->GetTextureOffset();
	if (ImGui::DragFloat2("Texture Offset", &texOffset.x, 1.0f))
	{
		gameObject->SetTextureOffset(texOffset);
	}

	// Размер текстуры
	glm::vec2 texSize = gameObject->GetTextureSize();
	if (ImGui::DragFloat2("Texture Size", &texSize.x, 1.0f, 0.0f, 2048.0f))
	{
		gameObject->SetTextureSize(texSize);
	}

	// Точка вращения (origin)
	glm::vec2 origin = gameObject->GetOrigin();
	if (ImGui::DragFloat2("Origin", &origin.x, 0.01f, 0.0f, 1.0f))
	{
		gameObject->SetOrigin(origin);
	}

	// Вращение
	float rotation = gameObject->GetRotation();
	if (ImGui::DragFloat("Rotation", &rotation, 1.0f, 0.0f, 360.0f))
	{
		gameObject->SetRotation(rotation);
	}

	// Цвет
	glm::vec4 color = gameObject->GetColor();
	if (ImGui::ColorEdit4("Color", &color.r))
	{
		gameObject->SetColor(color);
	}

	// Отражение
	bool flipX = gameObject->GetFlipX();
	if (ImGui::Checkbox("Flip X", &flipX))
	{
		gameObject->SetFlipX(flipX);
	}

	bool flipY = gameObject->GetFlipY();
	if (ImGui::Checkbox("Flip Y", &flipY))
	{
		gameObject->SetFlipY(flipY);
	}

	// Кнопка для сброса параметров
	if (ImGui::Button("Reset to Default"))
	{
		gameObject->SetPosition({10.0f, 10.0f});
		gameObject->SetSize({100.0f, 100.0f});
		gameObject->SetTextureOffset({0.0f, 0.0f});
		gameObject->SetTextureSize({0.0f, 0.0f});
		gameObject->SetOrigin({0.5f, 0.5f});
		gameObject->SetRotation(0.0f);
		gameObject->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		gameObject->SetFlipX(false);
		gameObject->SetFlipY(false);
	}

	ImGui::End();
}
