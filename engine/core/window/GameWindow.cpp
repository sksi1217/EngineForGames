#include <engine/core/window/GameWindow.hpp>

// ! Начальные методы
#pragma region Начальные методы

GameWindow::GameWindow()
{
	utils::Logger::info("Initializing GLFW...");

	if (!glfwInit())
	{
		utils::Logger::error("Failed to initialize GLFW");
		throw std::runtime_error("Failed to initialize GLFW");
	}

	// ! Настройка OpenGL контекста
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);				   // ? Указывает основную версию OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);				   // ? Указывает побочную версию OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ? Определяет, какой профиль OpenGL  использовать

	// ! Создание окна
	utils::Logger::info("Creating GLFW window...");
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

	// ! Проверка
	if (!m_Window)
	{
		utils::Logger::error("Failed to create GLFW window");
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(m_VSync ? 1 : 0);
}

GameWindow::~GameWindow()
{
	utils::Logger::info("Destroying GLFW window...");
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
#pragma endregion

// ! Основные операции
#pragma region Основные операции

void GameWindow::SwapBuffers()
{
	if (!m_Window)
	{
		utils::Logger::error("Invalid GLFW window handle...");
		throw std::runtime_error("Invalid GLFW window handle");
	}

	// ! Меняем буферы (двойная буферизация)
	glfwSwapBuffers(m_Window);
}

void GameWindow::Close()
{
	utils::Logger::info("Closing window...");
	glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

void GameWindow::SetVSync(bool enabled)
{
	m_VSync = enabled;
	utils::Logger::info("Setting VSync: " + std::string(enabled ? "ON" : "OFF"));
	glfwSwapInterval(enabled ? 1 : 0);
}

#pragma endregion

// ! Управление окном
#pragma region Управление окном

void GameWindow::SetTitle(const std::string &title)
{
	m_Title = title;
	utils::Logger::info("Setting window title to: " + title);
	glfwSetWindowTitle(m_Window, title.c_str());
}

void GameWindow::SetSize(int width, int height)
{
	utils::Logger::info("Setting window size to " + std::to_string(width) + "x" + std::to_string(height));
	m_Width = width;
	m_Height = height;
	glfwSetWindowSize(m_Window, width, height);
}

void GameWindow::SetPosition(int x, int y)
{
	utils::Logger::info("Setting window position to (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	glfwSetWindowPos(m_Window, x, y);
}

void GameWindow::Maximize()
{
	utils::Logger::info("Maximizing window");
	glfwMaximizeWindow(m_Window);
}

void GameWindow::Minimize()
{
	utils::Logger::info("Minimizing window");
	glfwIconifyWindow(m_Window);
}

void GameWindow::Restore()
{
	utils::Logger::info("Restoring window");
	glfwRestoreWindow(m_Window);
}

void GameWindow::SetIcon(const std::string &iconPath)
{
	GLFWimage icon;
	icon.pixels = stbi_load(iconPath.c_str(), &icon.width, &icon.height, nullptr, 4);
	if (icon.pixels)
	{
		utils::Logger::info("Setting window icon from file: " + iconPath);
		glfwSetWindowIcon(m_Window, 1, &icon);
		stbi_image_free(icon.pixels);
	}
	else
	{
		utils::Logger::warning("Failed to load window icon from: " + iconPath);
	}
}

#pragma endregion

// ! Состояние окна
#pragma region Состояние окна

bool GameWindow::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

bool GameWindow::IsVSyncEnabled()
{
	return m_VSync;
}

bool GameWindow::IsMaximized()
{
	return glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
}

bool GameWindow::IsMinimized()
{
	return glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED);
}

bool GameWindow::IsFocused()
{
	return glfwGetWindowAttrib(m_Window, GLFW_FOCUSED);
}

#pragma endregion

// ! Получение параметров
#pragma region Получение параметров

std::pair<int, int> GameWindow::GetSize()
{
	int width, height;
	glfwGetWindowSize(m_Window, &width, &height);
	return {width, height};
}

std::pair<int, int> GameWindow::GetPosition()
{
	int x, y;
	glfwGetWindowPos(m_Window, &x, &y);
	return {x, y};
}

std::pair<float, float> GameWindow::GetContentScale()
{
	float xscale, yscale;
	glfwGetWindowContentScale(m_Window, &xscale, &yscale);
	return {xscale, yscale};
}

std::string GameWindow::GetTitle()
{
	return m_Title;
}

GLFWwindow *GameWindow::GetWindowGLFW() { return m_Window; }

#pragma endregion

/*
// ! Настройки
#pragma region Настройки
void GameWindow::ApplySettings(const Settings &settings)
{
	// ! Графические настройки
	if (!m_Window)
	{
		utils::Logger::error("Window not initialized");
		return;
	}

	GLFWmonitor *monitor = nullptr;

	if (settings.graphics.fullscreen)
	{
		int monitorCount;
		GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

		if (monitorCount > 0 && settings.graphics.monitorIndex >= 0 &&
			settings.graphics.monitorIndex < monitorCount)
		{
			monitor = monitors[settings.graphics.monitorIndex];
		}
		else
		{
			std::stringstream ss;
			ss << "Invalid monitor index: " << settings.graphics.monitorIndex << "Using primary monitor." << settings.graphics.resolution.y;
			utils::Logger::error(ss.str());
			monitor = glfwGetPrimaryMonitor(); // fallback
		}
	}

	// Проверка поддержки разрешения в полноэкранном режиме
	if (monitor)
	{
		int modeCount;
		const GLFWvidmode *modes = glfwGetVideoModes(monitor, &modeCount);
		bool resolutionSupported = false;

		for (int i = 0; i < modeCount; ++i)
		{
			if (modes[i].width == settings.graphics.resolution.x &&
				modes[i].height == settings.graphics.resolution.y)
			{
				resolutionSupported = true;
				break;
			}
		}

		if (!resolutionSupported)
		{
			std::stringstream ss;
			ss << "Unsupported fullscreen resolution: " << settings.graphics.resolution.x << "x" << settings.graphics.resolution.y;
			utils::Logger::error(ss.str());
		}
	}

	GLFWwindow *currentContext = glfwGetCurrentContext();
	glfwMakeContextCurrent(m_Window);
	glFinish();

	try
	{
		const GLFWvidmode *targetMode = monitor ? glfwGetVideoMode(monitor) : nullptr;

		glfwSetWindowMonitor(
			m_Window,
			monitor,
			0, 0,
			settings.graphics.resolution.x,
			settings.graphics.resolution.y,
			monitor && targetMode ? targetMode->refreshRate : GLFW_DONT_CARE);

		glViewport(0, 0, settings.graphics.resolution.x, settings.graphics.resolution.y);
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << "Failed to apply window settings: " << e.what();
		utils::Logger::error(ss.str());
	}
	catch (...)
	{
		utils::Logger::error("Unknown error while applying window settings");
	}

	glfwMakeContextCurrent(currentContext);
	SetVSync(settings.graphics.vsync);

	// ! Применение звуковых настроек
	ApplyAudioSettings(settings.audio);

	// ! Применение настроек управления
	ApplyControlSettings(settings.controls);

	// ! Применение настроек интерфейса
	ApplyUISettings(settings.ui);

	// ! Применение игровых настроек
	ApplyGameSettings(settings.game);

	// ! Применение расширенных настроек
	ApplyAdvancedSettings(settings.advanced);

}

#pragma endregion
*/

/*
void GameWindow::ApplyAudioSettings(const AudioSettings &audio)
{

	// Пример: установка громкости через аудио-движок
	AudioEngine::SetMasterVolume(audio.masterVolume);
	AudioEngine::SetMusicVolume(audio.musicVolume);
	AudioEngine::SetEffectsVolume(audio.effectsVolume);
	AudioEngine::SetVoiceVolume(audio.voiceVolume);

	// Режим вывода
	switch (audio.outputMode)
	{
	case AudioSettings::MONO:
		AudioEngine::SetOutputMode(Mono);
		break;
	case AudioSettings::STEREO:
		AudioEngine::SetOutputMode(Stereo);
		break;
	case AudioSettings::SURROUND:
		AudioEngine::SetOutputMode(Surround);
		break;
	}

	AudioEngine::MuteAll(audio.muteAll);

}

void GameWindow::ApplyControlSettings(const ControlsSettings &controls)
{

	InputManager::SetKeyBinding("MoveForward", controls.moveForward);
	InputManager::SetKeyBinding("MoveBack", controls.moveBack);
	InputManager::SetKeyBinding("MoveLeft", controls.moveLeft);
	InputManager::SetKeyBinding("MoveRight", controls.moveRight);
	InputManager::SetKeyBinding("Jump", controls.jump);
	InputManager::SetKeyBinding("Interact", controls.interact);

	InputManager::SetMouseSensitivity(controls.mouseSensitivity);
	InputManager::SetInvertYAxis(controls.invertYAxis);

}

void GameWindow::ApplyUISettings(const UISettings &ui)
{

	UIRenderer::SetScale(ui.uiScale);
	UIRenderer::SetHudTransparency(ui.hudTransparency);
	UIRenderer::SetLanguage(ui.language);
	UIRenderer::SetShowHints(ui.showHints);
	UIRenderer::SetHideHUDInGame(ui.hideHUDInGame);
	UIRenderer::SetAutoScaleUI(ui.autoScaleUI);

}

void GameWindow::ApplyGameSettings(const GameSettings &game)
{

	GameplaySystem::SetAutoSaveEnabled(game.autoSave);
	MapSystem::SetMinimapVisible(game.showMinimap);

}

void GameWindow::ApplyAdvancedSettings(const AdvancedSettings &advanced){

	DebugConsole::SetEnabled(advanced.developerMode);
	Profiler::SetEnabled(advanced.developerMode);

}
*/
