#pragma once
#include "Includes.h"
#include "Events/Event.h"
#include "Window/WindowsInput.h"


struct WindowProp {
	std::string Title;
	unsigned int width, height;
	WindowProp(std::string s = "Render Window", unsigned int w = 1920, unsigned int h = 1080)
		:Title(s), width(w), height(h)
	{}
};
	//window interface for windows
	class Window {
	public:
		using EventCallbackFunc = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;

		//window parameters
		virtual void SetCallbackEvent(const EventCallbackFunc&) = 0;
		virtual void SetVsync(bool enable) = 0;
		virtual bool b_Vsync()const = 0;
		virtual void* GetNativeWindow() = 0;//gets the GLFWwindow pointer

		static std::unique_ptr<Window> Create(WindowProp& prop);
	};
