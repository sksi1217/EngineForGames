#pragma once

struct EngineState
{
	// ! Состояния окна/системы
	bool isRunning = true;
	bool isFocused = true;

	// ! Состояния отладки/пользовательского интерфейса
	bool showDebugUI = true;
	bool showSettingsWindow = false;

	// ! Входные состояния
	bool keys[512] = {false};
	bool mouseButtons[8] = {false};
	double mouseX = 0, mouseY = 0;

	bool isPaused = false;
	bool isFullscreen = false;
};
