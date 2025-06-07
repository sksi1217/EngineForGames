#include "ShaderManager.hpp"
#include <filesystem>
#include <iostream>

// ! == для ShaderKey
bool ShaderManager::ShaderKey::operator==(const ShaderKey &other) const
{
	return vertexPath == other.vertexPath && fragmentPath == other.fragmentPath;
}

// ! Хэш для ShaderKey
size_t ShaderManager::ShaderKeyHash::operator()(const ShaderKey &key) const
{
	return std::hash<std::string>{}(key.vertexPath) ^ (std::hash<std::string>{}(key.fragmentPath) << 1);
}

// ! Синглтон
ShaderManager &ShaderManager::Get()
{
	static ShaderManager instance;
	return instance;
}

void ShaderManager::Init()
{
	const std::string vertexPath = "assets/shaders/vertex.glsl";
	const std::string fragmentPath = "assets/shaders/fragment.frag";

	m_DefaultShader = LoadShader(vertexPath, fragmentPath);
	if (!m_DefaultShader)
	{
		utils::Logger::error("Failed to load default shader!");
		std::cerr << "Failed to load default shader!\n";
	}
}

std::shared_ptr<Shader> ShaderManager::LoadShader(const std::string &vertexPath, const std::string &fragmentPath)
{
	if (!std::filesystem::exists(vertexPath) || !std::filesystem::exists(fragmentPath))
	{
		utils::Logger::error("Shader files not found");
		std::cerr << "Shader files not found.\n";
		return nullptr;
	}

	ShaderKey key{vertexPath, fragmentPath};

	auto it = m_Cache.find(key);
	if (it != m_Cache.end())
	{
		return it->second;
	}

	auto shader = std::make_shared<Shader>();
	if (!shader->loadFromFile(vertexPath, fragmentPath))
	{
		utils::Logger::error("Failed to compile shader");
		std::cerr << "Failed to compile shader.\n";
		return nullptr;
	}

	m_Cache[key] = shader;
	return shader;
}
