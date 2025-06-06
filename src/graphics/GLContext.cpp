#include <src/graphics/GLContext.hpp>

void GLContext::Init(GLFWwindow *window)
{
	try
	{
		if (!window)
		{
			utils::Logger::error("Invalid GLFW window handle");
			throw std::runtime_error("Invalid GLFW window handle");
		}

		// Связываем контекст OpenGL с текущим окном GLFW
		glfwMakeContextCurrent(window);

		// Проверка версии OpenGL
		if (glGetString(GL_VERSION) == nullptr)
		{
			utils::Logger::error("Failed to initialize OpenGL context");
			throw std::runtime_error("Failed to initialize OpenGL context");
		}

		const char *glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
		if (glVersion)
		{
			utils::Logger::debug(std::string("OpenGL Version: ") + glVersion);
		}
		else
		{
			utils::Logger::error("Failed to get OpenGL version string.");
		}
	}
	catch (const std::exception &e)
	{
		std::string message = "OpenGL init error: " + std::string(e.what());

		utils::Logger::error(message);
		std::cerr << message << std::endl;

		return;
	}
}
