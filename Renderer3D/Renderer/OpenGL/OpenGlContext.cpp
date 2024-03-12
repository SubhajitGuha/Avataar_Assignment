#include "OpenGlContext.h"
#include "glad/glad.h"

OpenGlContext::OpenGlContext(std::shared_ptr<GLFWwindow> window)
	:m_GlfwWindow(window)
{
}

void OpenGlContext::Init()
{
	glfwMakeContextCurrent(m_GlfwWindow.get());
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr<<"Error in initilizing glad";
	}
}

void OpenGlContext::SwapBuffers()
{
	glfwSwapBuffers(m_GlfwWindow.get());
}
