#include <include/core/Settings.hpp>
#include <include/utils/Logger.hpp>
#include <fstream>
#include <libs/nlohmann/json.hpp>

using json = nlohmann::json;

const std::string Settings::SETTINGS_FILE = "settings.json";

Settings &Settings::Get()
{
	static Settings instance;
	return instance;
}

void Settings::Load()
{
	try
	{
		std::ifstream file(SETTINGS_FILE);
		if (file.good())
		{
			json j;
			file >> j;

			// Графика
			if (j.contains("graphics"))
			{
				auto &g = j["graphics"];
				graphics.resolution = {g.value("resolution_x", 1280), g.value("resolution_y", 720)};
				graphics.fullscreen = g.value("fullscreen", false);
				graphics.vsync = g.value("vsync", true);
				graphics.fpsLimit = g.value("fps_limit", 60);

				// Перечисления
				int textureQuality = g.value("texture_quality", static_cast<int>(GraphicsSettings::TextureQuality::HIGH));
				graphics.textureQuality = static_cast<GraphicsSettings::TextureQuality>(textureQuality);

				int antiAliasing = g.value("anti_aliasing", static_cast<int>(GraphicsSettings::AntiAliasing::FXAA));
				graphics.antiAliasing = static_cast<GraphicsSettings::AntiAliasing>(antiAliasing);

				graphics.pixelArtSmoothing = g.value("pixel_art_smoothing", false);
				graphics.showGrid = g.value("show_grid", false);
				graphics.gamma = g.value("gamma", 1.0f);
				graphics.brightness = g.value("brightness", 1.0f);
				graphics.contrast = g.value("contrast", 1.0f);
				graphics.debugWireframe = g.value("debug_wireframe", false);
				graphics.monitorIndex = g.value("monitor_index", 0);
			}

			// Звук
			if (j.contains("audio"))
			{
				auto &a = j["audio"];
				audio.masterVolume = a.value("master_volume", 1.0f);
				audio.musicVolume = a.value("music_volume", 0.8f);
				audio.effectsVolume = a.value("effects_volume", 0.8f);
				audio.voiceVolume = a.value("voice_volume", 0.9f);

				// int outputMode = a.value("output_mode", static_cast<int>(AudioSettings::STEREO));
				// audio.outputMode = static_cast<AudioSettings::AudioOutput>(outputMode);

				audio.muteAll = a.value("mute_all", false);
			}

			// Управление
			if (j.contains("controls"))
			{
				auto &c = j["controls"];
				controls.moveForward = c.value("move_forward", "W");
				controls.moveBack = c.value("move_back", "S");
				controls.moveLeft = c.value("move_left", "A");
				controls.moveRight = c.value("move_right", "D");
				controls.jump = c.value("jump", "SPACE");
				controls.interact = c.value("interact", "E");

				controls.mouseSensitivity = c.value("mouse_sensitivity", 1.0f);
				controls.invertYAxis = c.value("invert_y", false);
			}

			// Интерфейс
			if (j.contains("ui"))
			{
				auto &u = j["ui"];
				ui.uiScale = u.value("ui_scale", 1.0f);
				ui.hudTransparency = u.value("hud_transparency", 0.9f);
				ui.language = u.value("language", "en");
				ui.showHints = u.value("show_hints", true);
				ui.hideHUDInGame = u.value("hide_hud_in_game", false);
				ui.autoScaleUI = u.value("auto_scale_ui", true);
			}

			// Игра
			if (j.contains("game"))
			{
				auto &gm = j["game"];
				game.autoSave = gm.value("auto_save", true);
				game.showMinimap = gm.value("show_minimap", true);
			}

			// Расширенные
			if (j.contains("advanced"))
			{
				auto &ad = j["advanced"];
				advanced.developerMode = ad.value("developer_mode", false);
			}
		}
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << e.what();
		utils::Logger::error(ss.str());
	}
	catch (...)
	{
		utils::Logger::error("Unknown error while loading settings");
	}
}

void Settings::Save()
{
	try
	{
		json j;

		// Графика
		j["graphics"]["resolution_x"] = graphics.resolution.x;
		j["graphics"]["resolution_y"] = graphics.resolution.y;
		j["graphics"]["fullscreen"] = graphics.fullscreen;
		j["graphics"]["vsync"] = graphics.vsync;
		j["graphics"]["fps_limit"] = graphics.fpsLimit;

		j["graphics"]["texture_quality"] = static_cast<int>(graphics.textureQuality);
		j["graphics"]["anti_aliasing"] = static_cast<int>(graphics.antiAliasing);
		j["graphics"]["pixel_art_smoothing"] = graphics.pixelArtSmoothing;
		j["graphics"]["show_grid"] = graphics.showGrid;
		j["graphics"]["gamma"] = graphics.gamma;
		j["graphics"]["brightness"] = graphics.brightness;
		j["graphics"]["contrast"] = graphics.contrast;
		j["graphics"]["debug_wireframe"] = graphics.debugWireframe;
		j["graphics"]["monitor_index"] = graphics.monitorIndex;

		// Звук
		j["audio"]["master_volume"] = audio.masterVolume;
		j["audio"]["music_volume"] = audio.musicVolume;
		j["audio"]["effects_volume"] = audio.effectsVolume;
		j["audio"]["voice_volume"] = audio.voiceVolume;
		j["audio"]["output_mode"] = static_cast<int>(audio.outputMode);
		j["audio"]["mute_all"] = audio.muteAll;

		// Управление
		j["controls"]["move_forward"] = controls.moveForward;
		j["controls"]["move_back"] = controls.moveBack;
		j["controls"]["move_left"] = controls.moveLeft;
		j["controls"]["move_right"] = controls.moveRight;
		j["controls"]["jump"] = controls.jump;
		j["controls"]["interact"] = controls.interact;
		j["controls"]["mouse_sensitivity"] = controls.mouseSensitivity;
		j["controls"]["invert_y"] = controls.invertYAxis;

		// Интерфейс
		j["ui"]["ui_scale"] = ui.uiScale;
		j["ui"]["hud_transparency"] = ui.hudTransparency;
		j["ui"]["language"] = ui.language;
		j["ui"]["show_hints"] = ui.showHints;
		j["ui"]["hide_hud_in_game"] = ui.hideHUDInGame;
		j["ui"]["auto_scale_ui"] = ui.autoScaleUI;

		// Игра
		j["game"]["auto_save"] = game.autoSave;
		j["game"]["show_minimap"] = game.showMinimap;

		// Расширенные
		j["advanced"]["developer_mode"] = advanced.developerMode;

		// Сохраняем в файл
		std::ofstream file(SETTINGS_FILE);
		file << j.dump(4);
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << e.what();
		utils::Logger::error(ss.str());
	}
	catch (...)
	{
		utils::Logger::error("Unknown error while saving settings");
	}
}
