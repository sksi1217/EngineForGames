#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

class Shader; // Предварительное объявление

class ShaderLoader
{
public:
	// Загрузка шейдера (с кэшированием)
	static std::shared_ptr<Shader> loadShader(const std::string &vertexPath, const std::string &fragmentPath);

private:
	// Ключ для кэша: комбинация путей к вершинному и фрагментному шейдерам
	struct ShaderKey
	{
		std::string vertexPath;
		std::string fragmentPath;

		// Оператор сравнения для использования в unordered_map
		bool operator==(const ShaderKey &other) const
		{
			return vertexPath == other.vertexPath && fragmentPath == other.fragmentPath;
		}
	};

	// Хеш-функция для ShaderKey
	struct ShaderKeyHash
	{
		std::size_t operator()(const ShaderKey &key) const
		{
			return std::hash<std::string>()(key.vertexPath) ^ (std::hash<std::string>()(key.fragmentPath) << 1);
		}
	};

	/*
	 ? Кэш загруженных шейдеров
	 ! ShaderKey:  Тип ключа.
	 ! Shader *:  Тип значения.
	 ! ShaderKeyHash:  Хеш-функция.
	*/
	static std::unordered_map<ShaderKey, std::shared_ptr<Shader>, ShaderKeyHash > shaderCache;

	// Приватный конструктор (класс статический)
	ShaderLoader() = delete;
};
