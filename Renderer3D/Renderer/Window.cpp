#include"Window.h"
#include "Window/WindowsWindow.h"


std::unique_ptr<Window> Window::Create(WindowProp& prop)
{
	return std::make_unique<WindowsWindow>(prop);
}
