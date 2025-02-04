#pragma once

#include "IGPCH.hpp"

#include "Ignition/API.hpp"
#include "Ignition/Window.hpp"

namespace Ignition {
	class IGNITION_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> mWindow;
		bool mIsRunning = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
