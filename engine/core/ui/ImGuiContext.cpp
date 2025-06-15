#include <engine/core/ui/ImGuiContext.hpp>

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
	utils::Logger::info("Shutting down ImGui context");
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

void ImGuiContext::RenderDebugUI(std::unique_ptr<GameWindow> &m_Window, EventSystem &events, bool *showUI)
{
	// ! Создаём окно с debug информацией
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoCollapse);

	// ! --- Поле ввода текста ---
	ImGui::Separator();
	ImGui::Text("Input text:");
	if (ImGui::InputText("##input", m_InputText, sizeof(m_InputText), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		std::cout << "Введено: " << m_InputText << std::endl;
	}

	// ! FPS и время кадра
	ImGui::Text("FPS: %.1f", utils::Time::FPS());
	ImGui::Text("Frame time: %.3f ms", utils::Time::DeltaTime() * 1000.0f);

	// ! Разделитель
	ImGui::Separator();

	// ! Информация о GPU
	ImGui::Text("OpenGL:");
	ImGui::Text("  Vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("  Renderer: %s", glGetString(GL_RENDERER));
	ImGui::Text("  Version: %s", glGetString(GL_VERSION));

	// ! Разделитель
	ImGui::Separator();

	// ! Информация о системе
	ImGui::Text("Window:");
	int width, height;
	glfwGetWindowSize(m_Window->GetWindowGLFW(), &width, &height);
	ImGui::Text("  Size: %d x %d", width, height);
	ImGui::Text("  Last resize: %d x %d (%.2f sec ago)",
				events.m_LastWindowSize.w, events.m_LastWindowSize.h,
				static_cast<float>(glfwGetTime()) - events.m_LastWindowResizeTime);

	// ! Разделитель
	ImGui::Separator();

	// ! Информация о событиях
	ImGui::Text("Last Events:");

	// ! Клавиатура
	ImGui::Text("Keyboard:");
	ImGui::Text("  Key: %d, Action: %s, Mods: %d (%.2f sec ago)",
				events.m_LastKeyEvent.key,
				events.m_LastKeyEvent.action == GLFW_PRESS ? "PRESS" : events.m_LastKeyEvent.action == GLFW_RELEASE ? "RELEASE"
																													: "REPEAT",
				events.m_LastKeyEvent.mods,
				static_cast<float>(glfwGetTime()) - events.m_LastKeyEvent.time);

	// ! Мышь
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

	// ! Ввод текста
	ImGui::Text("Char Input:");
	if (events.m_LastCharInput.time > 0.0f) // если событие уже было
	{
		unsigned int codepoint = events.m_LastCharInput.character;
		if (codepoint > 0 && codepoint <= 0x7F)
		{
			char c = static_cast<char>(codepoint);
			ImGui::Text("  Char: %u ('%c') (%.2f sec ago)",
						codepoint,
						isprint(static_cast<unsigned char>(c)) ? c : '.',
						static_cast<float>(glfwGetTime()) - events.m_LastCharInput.time);
		}
		else
		{
			ImGui::Text("  Char: %u (non-ASCII or invalid)", codepoint);
		}
	}
	else
	{
		ImGui::Text("  Char: (no input yet)");
	}

	// ! Разделитель
	ImGui::Separator();

	// ! Кнопка для закрытия окна
	if (ImGui::Button("Close Debug"))
	{
		*showUI = false;
	}

	ImGui::End();
}

/*
void ImGuiContext::ShowSettingsWindow(bool *open, std::unique_ptr<GameWindow> &m_Window)
{
	if (!ImGui::Begin("Settings", open))
	{
		ImGui::End();
		return;
	}

	auto &settings = Settings::Get();

	if (ImGui::BeginTabBar("SettingsTabs"))
	{
		// ! Вкладка графики
		if (ImGui::BeginTabItem("Graphics"))
		{
			// ! Выбор разрешения
			static const std::vector<glm::ivec2> resolutions = {
				{1280, 720}, {1366, 768}, {1920, 1080}, {2560, 1440}};

			static int currentRes = 0;
			if (ImGui::BeginCombo("Resolution", (std::to_string(settings.graphics.resolution.x) + "x" + std::to_string(settings.graphics.resolution.y)).c_str()))
			{
				for (int i = 0; i < resolutions.size(); i++)
				{
					bool isSelected = (currentRes == i);
					std::string resLabel = std::to_string(resolutions[i].x) + "x" + std::to_string(resolutions[i].y);
					if (ImGui::Selectable(resLabel.c_str(), isSelected))
					{
						currentRes = i;
						settings.graphics.resolution = resolutions[i];
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fullscreen", &settings.graphics.fullscreen);
			ImGui::Checkbox("VSync", &settings.graphics.vsync);
			ImGui::SliderInt("FPS Limit", &settings.graphics.fpsLimit, 30, 240);

			// Качество текстур
			const char *textureQualityItems[] = {"Low", "Medium", "High", "Ultra"};
			int currentTextureQuality = static_cast<int>(settings.graphics.textureQuality);
			if (ImGui::Combo("Texture Quality", &currentTextureQuality, textureQualityItems, IM_ARRAYSIZE(textureQualityItems)))
			{
				settings.graphics.textureQuality = static_cast<GraphicsSettings::TextureQuality>(currentTextureQuality);
			}

			// Антиалиасинг
			const char *antiAliasingItems[] = {"None", "MSAA 2X", "MSAA 4X", "MSAA 8X", "FXAA"};
			int currentAntiAliasing = static_cast<int>(settings.graphics.antiAliasing);
			if (ImGui::Combo("Anti-Aliasing", &currentAntiAliasing, antiAliasingItems, IM_ARRAYSIZE(antiAliasingItems)))
			{
				settings.graphics.antiAliasing = static_cast<GraphicsSettings::AntiAliasing>(currentAntiAliasing);
			}

			ImGui::Checkbox("Pixel Art Smoothing", &settings.graphics.pixelArtSmoothing);
			ImGui::Checkbox("Show Grid", &settings.graphics.showGrid);
			ImGui::SliderFloat("Gamma", &settings.graphics.gamma, 0.5f, 2.0f);
			ImGui::SliderFloat("Brightness", &settings.graphics.brightness, 0.5f, 2.0f);
			ImGui::SliderFloat("Contrast", &settings.graphics.contrast, 0.5f, 2.0f);
			ImGui::Checkbox("Debug Wireframe", &settings.graphics.debugWireframe);

			int monitorCount;
			glfwGetMonitors(&monitorCount);

			ImGui::SliderInt("Monitor Index", &settings.graphics.monitorIndex, 0, monitorCount - 1);

			ImGui::EndTabItem();
		}

		// ! Вкладка звука
		if (ImGui::BeginTabItem("Audio"))
		{
			ImGui::SliderFloat("Master Volume", &settings.audio.masterVolume, 0.0f, 1.0f);
			ImGui::SliderFloat("Music Volume", &settings.audio.musicVolume, 0.0f, 1.0f);
			ImGui::SliderFloat("Effects Volume", &settings.audio.effectsVolume, 0.0f, 1.0f);
			ImGui::SliderFloat("Voice Volume", &settings.audio.voiceVolume, 0.0f, 1.0f);

			// Настройки вывода звука
			const char *audioOutputItems[] = {"Mono", "Stereo", "Surround"};
			int currentAudioOutput = static_cast<int>(settings.audio.outputMode);
			if (ImGui::Combo("Audio Output", &currentAudioOutput, audioOutputItems, IM_ARRAYSIZE(audioOutputItems)))
			{
				settings.audio.outputMode = static_cast<AudioSettings::AudioOutput>(currentAudioOutput);
			}

			ImGui::Checkbox("Mute All", &settings.audio.muteAll);

			// Кнопка теста звука
			if (ImGui::Button("Test Sound"))
			{
				// Здесь должен быть код для теста звука
			}

			ImGui::EndTabItem();
		}

		// ! Вкладка управления
		if (ImGui::BeginTabItem("Controls"))
		{
			// Здесь должен быть код для переназначения клавиш
			// Это сложная функциональность, требующая отдельной реализации

			ImGui::SliderFloat("Mouse Sensitivity", &settings.controls.mouseSensitivity, 0.1f, 5.0f);
			ImGui::Checkbox("Invert Y Axis", &settings.controls.invertYAxis);

			// Кнопка сброса управления
			if (ImGui::Button("Reset to Defaults"))
			{
				settings.controls = ControlsSettings();
			}

			ImGui::EndTabItem();
		}

		// ! Вкладка интерфейса
		if (ImGui::BeginTabItem("UI"))
		{
			ImGui::SliderFloat("UI Scale", &settings.ui.uiScale, 0.5f, 2.0f);
			ImGui::SliderFloat("HUD Transparency", &settings.ui.hudTransparency, 0.0f, 1.0f);

			// Выбор языка (упрощенно)
			const char *languages[] = {"English", "Russian", "French", "German", "Spanish"};
			static int currentLanguage = 0;
			ImGui::Combo("Language", &currentLanguage, languages, IM_ARRAYSIZE(languages));

			ImGui::Checkbox("Show Hints", &settings.ui.showHints);
			ImGui::Checkbox("Hide HUD In Game", &settings.ui.hideHUDInGame);
			ImGui::Checkbox("Auto Scale UI", &settings.ui.autoScaleUI);

			ImGui::EndTabItem();
		}

		// ! Вкладка игровых настроек
		if (ImGui::BeginTabItem("Game"))
		{
			ImGui::Checkbox("Auto Save", &settings.game.autoSave);
			ImGui::Checkbox("Show Minimap", &settings.game.showMinimap);

			ImGui::EndTabItem();
		}

		// ! Вкладка дополнительных настроек
		if (ImGui::BeginTabItem("Advanced"))
		{
			ImGui::Checkbox("Developer Mode", &settings.advanced.developerMode);

			// Кнопка сброса всех настроек
			if (ImGui::Button("Reset All Settings"))
			{
				// settings = Settings();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	if (ImGui::Button("Apply"))
	{
		settings.Save();
		// ! Применить настройки
		// m_Window->ApplySettings(settings);
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
	{
		settings.Load(); // Вернуть предыдущие настройки
		*open = false;
	}

	ImGui::End();
}
*/

void ImGuiContext::RenderObjectUI(Transform &transform)
{
	ImGui::Begin("Object Properties", nullptr, ImGuiWindowFlags_NoCollapse);

	if (ImGui::DragFloat2("Position", &transform.Position.x(), 1.0f))
	{
		// Изменения уже внесены благодаря ссылке
	}

	if (ImGui::DragFloat2("Origin", &transform.Origin.x, 0.01f, 0.0f, 0.5f))
	{
		// То же самое
	}

	ImGui::End();
}
