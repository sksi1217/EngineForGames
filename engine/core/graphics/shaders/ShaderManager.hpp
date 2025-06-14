#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <engine/core/graphics/shaders/Shader.hpp>
#include <engine/core/utils/Logger.hpp>

class Shader;

class ShaderManager
{
public:
	static ShaderManager &Get();

	// Инициализация — загружаем дефолтный шейдер
	void Init();

	// Получение дефолтного шейдера
	std::shared_ptr<Shader> GetDefaultShader() const { return m_DefaultShader; }

	// Загрузка любого шейдера с кэшированием
	std::shared_ptr<Shader> LoadShader(const std::string &vertexPath, const std::string &fragmentPath);

private:
	struct ShaderKey
	{
		std::string vertexPath;
		std::string fragmentPath;

		bool operator==(const ShaderKey &other) const;
	};

	struct ShaderKeyHash
	{
		size_t operator()(const ShaderKey &key) const;
	};

	std::unordered_map<ShaderKey, std::shared_ptr<Shader>, ShaderKeyHash> m_Cache;

	std::shared_ptr<Shader> m_DefaultShader;
};
