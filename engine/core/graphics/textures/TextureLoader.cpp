// TextureLoader.cpp
#include <engine/core/graphics/textures/TextureLoader.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <extern/stb/stb_image.h>

// Инициализация статического кэша
std::unordered_map<std::string, std::shared_ptr<Texture2D>> TextureLoader::textureCache;

std::shared_ptr<Texture2D> TextureLoader::loadTexture(const std::string &filePath)
{
	// Проверяем, есть ли текстура в кэше
	auto it = textureCache.find(filePath);
	if (it != textureCache.end())
	{
		std::cout << "Returning cached texture for: " << filePath << std::endl;
		return it->second; // Возвращаем кэшированную текстуру
	}

	// Создаем новую текстуру
	auto texture = std::make_unique<Texture2D>();
	if (!createTexture(filePath, *texture))
	{
		std::cerr << "Failed to load texture: " << filePath << std::endl;
		return nullptr;
	}

	// Добавляем текстуру в кэш
	textureCache[filePath] = std::move(texture);
	return textureCache[filePath];
}

bool TextureLoader::createTexture(const std::string &filePath, Texture2D &texture)
{
	int width, height, channels;
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		std::cerr << "Failed to load image: " << filePath << std::endl;
		return false;
	}

	// Создаем текстуру
	glGenTextures(1, &texture.id);

	// Делаем текстуру активной
	glBindTexture(GL_TEXTURE_2D, texture.id);

	// Настройка параметров текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	 // Линейная фильтрация
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	 // Линейная фильтрация
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Обрезка по краям
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Обрезка по краям

	// Загрузка данных текстуры
	GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	// Отвязываем текстуру
	glBindTexture(GL_TEXTURE_2D, 0);

	// Сохраняем размеры текстуры
	texture.width = width;
	texture.height = height;

	// Освобождаем данные изображения
	stbi_image_free(data);
	return true;
}

void TextureLoader::clearCache()
{
	textureCache.clear();
}
