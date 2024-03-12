#include "Application.h"
#include "EntryPoint.h"
#include "Window/WindowsInput.h"
#include "Window/InputCodes.h"
#include <chrono>

#define BIND_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	Application* Application::getApplication;
	Application* CreateApplication() //creates application (called in the main function)
	{
		return new Application();
	}

	Application::Application()
	{
		WindowProp prop("Render Window", 512,512);
		m_window = Window::Create(prop);
		m_window->SetCallbackEvent(BIND_FN(OnEvent));

		renderer = std::make_shared<OGLRenderer>(512, 512); //create the renderer
		camera = std::make_shared<Camera>(512, 512); //create a camera
		getApplication = this;
		
		//set the world-space positions for the spheres
		int offset = 3; //spacing in between 2 spheres
		SphereParameters param;
		for (int y = 0; y < 6; y++)
		{
			for (int x = 0; x < 6; x++)
			{
				param.position = { offset * (x - 3) + 1,offset * (y - 3) + 1,0 };
				param.diffuse_factor = x / 6.0f;
				param.specular_factor = y / 6.0f;
				spheres.push_back(param);
			}
		}
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispach(e);
		dispach.Dispatch<WindowCloseEvent>(BIND_FN(closeWindow));
		dispach.Dispatch<MouseButtonPressed>(BIND_FN(highlightSphere));
		dispach.Dispatch<WindowResizeEvent>([&](WindowResizeEvent e) {
			window_width = e.GetWidth(); window_height = e.GetHeight();
			renderer->SetViewport(e.GetWidth(), e.GetHeight());
			camera->SetViewportSize((float)e.GetWidth() / e.GetHeight());
			return false; 
			});
		std::cout<<e<<std::endl;		
	}

	
	bool Application::RaySphereIntersect(const glm::vec3& ray_pos, const glm::vec3& sphere_pos, float radius)
	{
		glm::vec3 ac = ray_pos - sphere_pos;
		auto ray_dir = glm::normalize(camera->GetViewDirection());
		float a = glm::dot(ray_dir, ray_dir);
		float b = 2 * glm::dot(ray_dir, ac);
		float c = glm::dot(ac, ac) - glm::pow(radius, 2);
		float discriminant = b * b - 4 * a * c;

		if (discriminant >= 0)
			return true;
		return false;
	}

	bool Application::closeWindow(WindowCloseEvent& EventClose)
	{
		m_Running = false;
		return true;
	}

	//event callback function that highlights the spheres when clicked
	bool Application::highlightSphere(MouseButtonPressed& e)
	{
		if (e.GetMouseButton() != APP_MOUSE_BUTTON_1)
			return false;
		
		auto cursor_pos = Input::GetCursorPosition();
		//convert the window space positions to clip-space positions
		glm::vec4 rayOrigin_clipSpace(
			((float)cursor_pos.first / (float)m_window->GetWidth() - 0.5f) * 2.0f,
			-((float)cursor_pos.second / (float)m_window->GetHeight() - 0.5f) * 2.0f,
			-1.0,1.0f); //[-1,1] range

		glm::vec4 rayOrigin_viewSpace = glm::inverse(camera->GetProjectionMatrix()) * rayOrigin_clipSpace;

		glm::vec4 rayOrigin_worldSpace = glm::inverse(camera->GetViewMatrix()) * rayOrigin_viewSpace;
		rayOrigin_worldSpace.z = camera->GetCameraPosition().z; //set the z-value of the ray same as the camera z-value

		for (int i = 0; i < spheres.size(); i++)
		{
			if (RaySphereIntersect(rayOrigin_worldSpace, spheres[i].position)) //if the ray intersects the sphere
			{
				if (spheres[i].isHit)
					spheres[i].isHit = 0;
				else
					spheres[i].isHit = 1;
				return false;
			}
		}

		//if a click was made on the region other than the spheres then undo the highlights
		for (int i = 0; i < spheres.size(); i++)
			spheres[i].isHit = 0;

		return false;
	}

	void Application::Run()
	{
		while (m_Running) //render loop
		{
			if (Input::IsKeyPressed(APP_KEY_ESCAPE)) //exit when Esc is pressed
				m_Running = false;

			m_window->OnUpdate();

			float time = glfwGetTime();
			float deltaTime = time - m_LastFrameTime;//this is the delta time (time btn last and present frame or time required to render a frame)
			m_LastFrameTime = time;

			camera->OnUpdate(deltaTime);
			
			renderer->ClearColor({ 0.5,0.2,0.2,1.0 });
			renderer->Clear();

			for (int i = 0; i < spheres.size(); i++)
			{
				auto transform = glm::translate(glm::mat4(1.0), spheres[i].position); //create the translation matrix
				renderer->Draw(*camera, transform, spheres[i].diffuse_factor, spheres[i].specular_factor, spheres[i].isHit);
			}
		}
	}

	
