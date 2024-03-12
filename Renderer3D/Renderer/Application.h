#pragma once
#include"Window.h"
#include "Events/ApplicationEvent.h"
#include "Includes.h"
#include "OpenGL/OpenGLRenderer.h"
#include "Camera/Camera.h"

class Application {
private:
	struct SphereParameters
	{
		glm::vec3 position;
		float diffuse_factor;
		float specular_factor;
		int isHit = 0;
	};
public:
	Application();

	virtual ~Application();

	void Run();

	void OnEvent(Event& e);

	inline Window& GetWindow() { return *m_window; }

	static inline Application& Get() { return *getApplication; }

	bool RaySphereIntersect(const glm::vec3& ray_pos, const glm::vec3& sphere_pos, float radius = 1.0f);

public:
	std::shared_ptr<OGLRenderer> renderer;
	std::shared_ptr<Camera> camera;
private:
	std::unique_ptr<Window> m_window;
	uint32_t window_width, window_height;
	bool m_Running = true;
	bool closeWindow(WindowCloseEvent&);
	bool highlightSphere(MouseButtonPressed&);
	static Application* getApplication;
	float m_LastFrameTime;
	std::vector<SphereParameters> spheres;
};