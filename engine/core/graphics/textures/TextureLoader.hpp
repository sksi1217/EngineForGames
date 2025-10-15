#pragma once
#include <string>

#include <engine/LightEngine.hpp>

// #include <GL/gl.h>
#include <iostream>
#include <memory>
#include <unordered_map>

struct Texture2D;

class TextureLoader
{
public:
	// ! Загрузка текстуры (с кэшированием)
	static std::shared_ptr<Texture2D> LoadTexture(const std::string &filePath);

	// ! Очистка кэша текстур
	static void ClearCache();

private:
	// ! Создание текстуры в OpenGL
	static bool CreateTexture(const std::string &filePath, Texture2D &texture);

	// ! Кэш текстур
	static std::unordered_map<std::string, std::shared_ptr<Texture2D>> textureCache;
};
