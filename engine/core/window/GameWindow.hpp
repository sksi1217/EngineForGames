#pragma once

// #include <engine/engineapi.hpp>

// #include <engine/core/ui/Settings.hpp>

#include <engine/core/utils/Logger.hpp>

#include <GLFW/glfw3.h>

class GameWindow
{
public:
	GameWindow();
	~GameWindow();

	// ! Основные операции
	void SwapBuffers();
	void Close();
	void SetVSync(bool enabled);

	// ! Управление окном
	void SetTitle(const std::string &title);
	void SetSize(int width, int height);
	void SetPosition(int x, int y);
	void Maximize();
	void Minimize();
	void Restore();
	void SetIcon(const std::string &iconPath);

	// ! Состояние окна
	bool ShouldClose();
	bool IsVSyncEnabled();
	bool IsMaximized();
	bool IsMinimized();
	bool IsFocused();

	// ! Получение параметров
	std::pair<int, int> GetSize();
	std::pair<int, int> GetPosition();
	std::pair<float, float> GetContentScale();
	std::string GetTitle();
	GLFWwindow *GetWindowGLFW();

	/*
	// ! применения настроек ИСПРАВИТЬ
	void ApplySettings(const Settings &settings);

	void ApplyAudioSettings(const AudioSettings &audio);
	void ApplyControlSettings(const ControlsSettings &controls);
	void ApplyUISettings(const UISettings &ui);
	void ApplyGameSettings(const GameSettings &game);
	void ApplyAdvancedSettings(const AdvancedSettings &advanced);
	*/

private:
	GLFWwindow *m_Window;
	std::string m_Title = "Window";
	int m_Width = 1280, m_Height = 720;
	bool m_VSync;
};
