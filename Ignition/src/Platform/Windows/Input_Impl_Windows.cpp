#include "IGPCH.hpp"
#include "Ignition/Core/Input.hpp"

#include "Ignition/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace Ignition {

	bool Core::Input::IsKeyPressed(int keycode) {
		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Core::Input::IsMouseButtonPressed(int button) {
		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Core::Input::GetCursorPosition() {
		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Core::Input::GetCursorX() {
		auto [x, y] = GetCursorPosition();
		return x;
	}

	float Core::Input::GetCursorY() {
		auto [x, y] = GetCursorPosition();
		return y;
	}
}