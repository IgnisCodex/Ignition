#include "IGPCH.hpp"

#include "Ignition/Application.hpp"
#include "Ignition/Log.hpp"
#include "Ignition/Events/Event.hpp"

#include <GLFW/glfw3.h>

namespace Ignition {
	Application::Application() {
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application() {}

	void Application::Run() {
		IG_CORE_INFO("Started!");
		while (mIsRunning) {
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			mWindow->OnUpdate();
		}
	}

	bool Application::End(Events::WindowCloseEvent& event) {
		mIsRunning = false;
		return true;
	}

	void Application::OnEvent(Events::Event& event) {
		Events::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Events::WindowCloseEvent>(std::bind(&Application::End, this, std::placeholders::_1));
		
		
		if (event.IsType(Events::EventType::Window))
			IG_CORE_TRACE("{}", event.GetName());
	}
}