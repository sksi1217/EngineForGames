#include <engine/core/graphics/shaders/Shader.hpp>

Shader::Shader() : id(0) {}

Shader::~Shader()
{
	utils::Logger::info("Delete Shader!");
	if (id != 0)
	{
		glDeleteProgram(id);
	}
}

bool Shader::loadFromFile(const std::string &vertexPath, const std::string &fragmentPath)
{
	// ! Загрузка исходного кода шейдеров
	std::string vertexSource = loadShaderSource(vertexPath);
	std::string fragmentSource = loadShaderSource(fragmentPath);

	if (vertexSource.empty() || fragmentSource.empty())
	{
		utils::Logger::error("Failed to load shader source files.");
		return false;
	}

	// ! Компиляция шейдеров
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	if (vertexShader == 0 || fragmentShader == 0)
	{
		return false;
	}

	// ! Создание программы
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	// ! Проверка успешности связывания
	GLint success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::stringstream ss;
		ss << infoLog;
		utils::Logger::error("Shader program linking failed: " + ss.str());
		return false;
	}

	// ! Очистка скомпилированных шейдеров
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

GLuint Shader::compileShader(GLenum type, const std::string &source)
{
	GLuint shader = glCreateShader(type);
	if (shader == 0)
	{
		std::stringstream ss;
		ss << type;
		utils::Logger::error("Failed to create shader of type: " + ss.str());
		return 0;
	}

	const char *sourceCStr = source.c_str();
	glShaderSource(shader, 1, &sourceCStr, nullptr);
	glCompileShader(shader);

	// ! Проверка успешности компиляции
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::stringstream ss;
		ss << infoLog;
		utils::Logger::error("Shader compilation failed: " + ss.str());
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
		std::stringstream ss;
		ss << filePath;
		utils::Logger::error("Failed to open file: " + ss.str());
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

void Shader::Use() const
{
	glUseProgram(id);
}

GLuint Shader::getProgramID() const
{
	return id;
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		std::stringstream ss;
		ss << name;
		utils::Logger::error("Uniform variable not found: " + ss.str());
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		std::stringstream ss;
		ss << name;
		utils::Logger::error("Uniform variable not found: " + ss.str());
		return;
	}

	glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		std::stringstream ss;
		ss << name;
		utils::Logger::error("Uniform variable not found: " + ss.str());
		return;
	}
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		std::stringstream ss;
		ss << name;
		utils::Logger::error("Uniform variable not found: " + ss.str());
		return;
	}
	glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setFloat(const std::string &name, float value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		std::stringstream ss;
		ss << name;
		utils::Logger::error("Uniform variable not found: " + ss.str());
		return;
	}
	glUniform1f(location, value);
}
