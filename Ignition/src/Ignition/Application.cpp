#include "Application.hpp"
#include "Log.hpp"

#include <GLFW/glfw3.h>

namespace Ignition {
	Application::Application() {
		mWindow = std::unique_ptr<Window>(Window::Create());
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
}