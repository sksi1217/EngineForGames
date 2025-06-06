#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp> // Для работы с матрицами
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	~Shader();

	// Загрузка шейдеров из файлов
	bool loadFromFile(const std::string &vertexPath, const std::string &fragmentPath);

	// Использование программы
	void Use() const;

	// Получение ID программы
	GLuint getProgramID() const;

	// Установка uniform-переменной типа mat4
	void setMat4(const std::string &name, const glm::mat4 &matrix) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;

private:
	GLuint programID; // ID шейдерной программы в OpenGL

	// Вспомогательные методы
	GLuint compileShader(GLenum type, const std::string &source);
	std::string loadShaderSource(const std::string &filePath);
};
