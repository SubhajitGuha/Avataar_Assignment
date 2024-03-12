#pragma once
#include "Includes.h"
#include "GLFW/glfw3.h"


class OpenGlContext
{
public:
	OpenGlContext(std::shared_ptr<GLFWwindow>);
	void Init();
	void SwapBuffers();
private:
	std::shared_ptr<GLFWwindow> m_GlfwWindow;
};


