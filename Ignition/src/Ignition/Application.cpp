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
			for (Core::Layer* layer : mLayerStack)
				layer->OnUpdate();
			mWindow->OnUpdate();
		}
	}

	void Application::PushLayer(Core::Layer* layer) {
		mLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Core::Layer* layer) {
		mLayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Events::Event& event) {
		Events::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Events::WindowCloseEvent>(std::bind(&Application::End, this, std::placeholders::_1));
		
		for (auto it = mLayerStack.end(); it != mLayerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.IsHandled)
				break;
		}
	}

	// Private Methods
	bool Application::End(Events::WindowCloseEvent& event) {
		mIsRunning = false;
		return true;
	}
}