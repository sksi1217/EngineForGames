#pragma once

#include <src/graphics/Texture2D.hpp>

#include <unordered_map>
#include <string>
#include <memory>

class TextureLoader
{
public:
	// Загрузка текстуры (с кэшированием)
	static std::shared_ptr<Texture2D> loadTexture(const std::string &filePath);

	// Очистка кэша текстур
	static void clearCache();

private:
	// Создание текстуры в OpenGL
	static bool createTexture(const std::string &filePath, Texture2D &texture);

	// Кэш текстур
	static std::unordered_map<std::string, std::shared_ptr<Texture2D>> textureCache;
};
