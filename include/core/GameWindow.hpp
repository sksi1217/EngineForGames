#pragma once

#include <GLFW/glfw3.h>
#include <include/utils/Logger.hpp>
#include <include/stb_image.h>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <string>

#include <include/core/Settings.hpp>

class GameWindow
{
public:
	GameWindow() = default; // ИСПРАВИТЬ (нужно чтобы было только 1 метод GameWindow)
	GameWindow(const std::string &title, int width, int height);
	~GameWindow();

	static GameWindow &Get();

	// ! Основные операции
	void SwapBuffers() const;
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
	bool ShouldClose() const;
	bool IsVSyncEnabled() const;
	bool IsMaximized() const;
	bool IsMinimized() const;
	bool IsFocused() const;

	// ! Получение параметров
	std::pair<int, int> GetSize() const;
	std::pair<int, int> GetPosition() const;
	std::pair<float, float> GetContentScale() const;
	std::string GetTitle() const;
	inline GLFWwindow *GetWindow() const { return m_Window; }

	// ! применения настроек ИСПРАВИТЬ
	void ApplySettings(const Settings &settings);

	void ApplyAudioSettings(const AudioSettings &audio);
	void ApplyControlSettings(const ControlsSettings &controls);
	void ApplyUISettings(const UISettings &ui);
	void ApplyGameSettings(const GameSettings &game);
	void ApplyAdvancedSettings(const AdvancedSettings &advanced);

private:
	GLFWwindow *m_Window;
	std::string m_Title;
	int m_Width, m_Height;
	bool m_VSync;
};
