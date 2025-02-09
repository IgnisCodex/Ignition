#include "IGPCH.hpp"

#include "Ignition/Core/Application.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Events/Event.hpp"
#include "Ignition/UI/ImGuiLayer.hpp"

#include <glad/glad.h>

namespace Ignition::Core {

	Application* Application::sInstance = nullptr;

	Application::Application() {
		IG_CORE_ASSERT(!sInstance, "Application Instance Already Exists!");
		sInstance = this;

		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		mImGuiLayer = new UI::ImGuiLayer();
		PushOverlay(mImGuiLayer);
	}

	Application::~Application() {}

	void Application::Run() {
		IG_CORE_INFO("Started!");

		while (mIsRunning) {
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Core::Layer* layer : mLayerStack)
				layer->OnUpdate();

			mImGuiLayer->Begin();
			for (Core::Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->End();

			mWindow->OnUpdate();
		}
	}

	void Application::PushLayer(Core::Layer* layer) {
		mLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Core::Layer* layer) {
		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Events::Event& event) {
		Events::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Events::WindowCloseEvent>(std::bind(&Application::OnWindowCloseEvent, this, std::placeholders::_1));

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.IsHandled)
				break;
		}
	}

	// Private Methods
	bool Application::OnWindowCloseEvent(Events::WindowCloseEvent& event) {
		mIsRunning = false;
		return true;
	}
}
