#include "IGPCH.hpp"

#include "Platform/Windows/Input_Impl_Windows.hpp"

#include "Ignition/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace Ignition {

	Input* Input::sInstance = new Input_Impl_Windows();
	
	bool Input_Impl_Windows::IsKeyPressed_Impl(int keycode) {
		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input_Impl_Windows::IsMouseButtonPressed_Impl(int button) {
		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input_Impl_Windows::GetCursorPosition_Impl() {
		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input_Impl_Windows::GetCursorX_Impl() {
		auto [x, y] = GetCursorPosition_Impl();
		return x;
	}

	float Input_Impl_Windows::GetCursorY_Impl() {
		auto [x, y] = GetCursorPosition_Impl();
		return y;
	}
}