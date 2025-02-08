#pragma once

#include "IGPCH.hpp"

#include "Ignition/API.hpp"
#include "Ignition/Window.hpp"
#include "Ignition/Core/LayerStack.hpp"
#include "Ignition/Events/Event.hpp"
#include "Ignition/Events/ApplicationEvent.hpp"

namespace Ignition {
	class IGNITION_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Events::Event& event);

		void PushLayer(Core::Layer* layer);
		void PushOverlay(Core::Layer* layer);

	private:
		bool End(Events::WindowCloseEvent& event);

		std::unique_ptr<Window> mWindow;
		bool mIsRunning = true;
		Core::LayerStack mLayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
