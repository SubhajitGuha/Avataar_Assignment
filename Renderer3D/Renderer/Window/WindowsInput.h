#pragma once
#include "Input.h"
#include "GLFW/glfw3.h"

class WindowsInput : public Input
{
public:
	bool IsKeyPressedImpl(int keyCode) override;
	bool IsMouseButtonPressed(int Button) override;
	std::pair<double, double> GetMousePos() override;
};
