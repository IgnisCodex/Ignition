#pragma once

#include "IGPCH.hpp"

#include "Ignition/API.hpp"
#include "Ignition/Window.hpp"
#include "Ignition/Events/Event.hpp"
#include "Ignition/Events/ApplicationEvent.hpp"

namespace Ignition {
	class IGNITION_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
		bool End(Events::WindowCloseEvent& event);

		void OnEvent(Events::Event& event);

	private:
		std::unique_ptr<Window> mWindow;
		bool mIsRunning = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
