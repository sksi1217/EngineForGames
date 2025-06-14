#pragma once

#include <GLFW/glfw3.h>
#include <GL/glx.h>
#include <stdexcept>

#include <engine/core/utils/Logger.hpp>
#include <iostream>

class GLContext
{
public:
	void Init(GLFWwindow *window);
};
