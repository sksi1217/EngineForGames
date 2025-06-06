#include <src/graphics/Shader.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : programID(0) {}

Shader::~Shader()
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
	}
}

bool Shader::loadFromFile(const std::string &vertexPath, const std::string &fragmentPath)
{
	// Загрузка исходного кода шейдеров
	std::string vertexSource = loadShaderSource(vertexPath);
	std::string fragmentSource = loadShaderSource(fragmentPath);

	if (vertexSource.empty() || fragmentSource.empty())
	{
		std::cerr << "Failed to load shader source files." << std::endl;
		return false;
	}

	// Компиляция шейдеров
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	if (vertexShader == 0 || fragmentShader == 0)
	{
		return false;
	}

	// Создание программы
	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	// Проверка успешности связывания
	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		std::cerr << "Shader program linking failed: " << infoLog << std::endl;
		return false;
	}

	// Очистка скомпилированных шейдеров
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

GLuint Shader::compileShader(GLenum type, const std::string &source)
{
	GLuint shader = glCreateShader(type);
	if (shader == 0)
	{
		std::cerr << "Failed to create shader of type: " << type << std::endl;
		return 0;
	}

	const char *sourceCStr = source.c_str();
	glShaderSource(shader, 1, &sourceCStr, nullptr);
	glCompileShader(shader);

	// Проверка успешности компиляции
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compilation failed: " << infoLog << std::endl;
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

std::string Shader::loadShaderSource(const std::string &filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

void Shader::Use() const
{
	glUseProgram(programID);
}

GLuint Shader::getProgramID() const
{
	return programID;
}

// Новый метод для установки uniform-переменной типа mat4
void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) const
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Uniform variable not found: " << name << std::endl;
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Uniform variable not found: " << name << std::endl;
		return;
	}
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	// Используем glGetUniformLocation для получения расположения uniform-переменной
	int location = glGetUniformLocation(programID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Uniform variable '" << name << "' not found!" << std::endl;
		return;
	}

	// Устанавливаем значение vec2 с помощью glUniform2fv
	glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::setFloat(const std::string &name, float value) const
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Uniform variable not found: " << name << std::endl;
		return;
	}
	glUniform1f(location, value);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Uniform variable not found: " << name << std::endl;
		return;
	}
	glUniform4fv(location, 1, glm::value_ptr(value));
}
