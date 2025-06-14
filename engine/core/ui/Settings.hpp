#pragma once
#include <string>
#include <glm/glm.hpp>

/*
/
... Графические настройки ...
/ Разрешение экрана +
/ Полноэкранный режим +
/ VSync (вертикальная синхронизация) +
/ FPS Limiter
/ Качество текстур
/ Антиалиасинг (MSAA, FXAA и др.)
/ Сглаживание пиксельной графики (Pixel Art Smoothing)
/ Отображение сетки / грид
/ Гамма / Яркость / Контрастность
/ Отрисовка границ объектов (Debug Mode)
/
... Звуковые настройки ...
/ Общая громкость
/ Громкость музыки
/ Громкость звуковых эффектов
/ Громкость голоса / диалогов
/ Тип вывода звука (Моно / Стерео)
/ Тест воспроизведения звука (Кнопка для проверки звука)
/ Отключить звук полностью
/
... Управление ...
/ Переназначение клавиш
/ Чувствительность мыши
/ Инвертировать вертикальное управление
/ Сброс до стандартных привязок (Кнопка сброса управления к умолчанию)
/
... Интерфейс ...
/ Масштаб интерфейса
/ Прозрачность панелей / HUD
/ Язык интерфейса
/ Отображение подсказок
/ Скрыть HUD во время игры
/ Автомасштабирование UI
/
... Игровые настройки ...
/ Сохранять прогресс автоматически (Сохранение после каждого события или уровня)
/ Отображение карты / миникарты (Вкл/Выкл)
/
... Дополнительно ...
/ Режим разработчика (Консоль команд, отладочные инструменты)
/ Сброс всех настроек (Кнопка сброса ко всем значениям по умолчанию)
/
*/

struct GraphicsSettings
{
	// Основные настройки
	glm::ivec2 resolution = {1280, 720};

	bool fullscreen = false;
	bool vsync = true;
	int fpsLimit = 60;

	// Качество графики
	enum class TextureQuality
	{
		LOW,
		MEDIUM,
		HIGH,
		ULTRA
	} textureQuality = TextureQuality::HIGH;

	enum class AntiAliasing
	{
		NONE,
		MSAA_2X,
		MSAA_4X,
		MSAA_8X,
		FXAA
	} antiAliasing = AntiAliasing::FXAA;
	bool pixelArtSmoothing = false;

	// Отладочные/дополнительные
	bool showGrid = false;
	float gamma = 1.0f;
	float brightness = 1.0f;
	float contrast = 1.0f;
	bool debugWireframe = false;

	// Настройка монитора
	int monitorIndex = 0;
};

struct AudioSettings
{
	// Громкость
	float masterVolume = 1.0f;
	float musicVolume = 0.8f;
	float effectsVolume = 0.8f;
	float voiceVolume = 0.9f;

	// Настройки вывода
	enum class AudioOutput
	{
		MONO,
		STEREO,
		SURROUND
	} outputMode = AudioOutput::STEREO;
	bool muteAll = false;
};

struct ControlsSettings
{
	// Клавиатура (пример)
	std::string moveForward = "W";
	std::string moveBack = "S";
	std::string moveLeft = "A";
	std::string moveRight = "D";
	std::string jump = "SPACE";
	std::string interact = "E";

	// Мышь
	float mouseSensitivity = 1.0f;
	bool invertYAxis = false;
};

struct UISettings
{
	float uiScale = 1.0f;
	float hudTransparency = 0.9f;
	std::string language = "en";
	bool showHints = true;
	bool hideHUDInGame = false;
	bool autoScaleUI = true;
};

struct GameSettings
{
	bool autoSave = true;
	bool showMinimap = true;
};

struct AdvancedSettings
{
	bool developerMode = false;
};

class Settings
{
public:
	static Settings &Get();

	void Load();
	void Save();

	GraphicsSettings graphics;
	AudioSettings audio;
	ControlsSettings controls;
	UISettings ui;
	GameSettings game;
	AdvancedSettings advanced;

private:
	Settings() = default;
	static const std::string SETTINGS_FILE;
};
