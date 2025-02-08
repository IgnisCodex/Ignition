#include "IGPCH.hpp"
#include "Platform/Windows/Window_Impl_Windows.hpp"
#include "Ignition/Log.hpp"

#include "Ignition/Events/ApplicationEvent.hpp"
#include "Ignition/Events/KeyboardEvent.hpp"
#include "Ignition/Events/MouseEvent.hpp"

#include <glad/glad.h>

namespace Ignition {

	static bool IsGLFWInitalised = false;

	Window* Window::Create(const WindowProperties& properties) {
		return new Window_Impl_Windows(properties);
	}

	Window_Impl_Windows::Window_Impl_Windows(const WindowProperties& properties) {
		Init(properties);
	}

	Window_Impl_Windows::~Window_Impl_Windows() {
		End();
	}

	void Window_Impl_Windows::Init(const WindowProperties& properties) {
		mWindowData.Title = properties.Title;
		mWindowData.Width = properties.Width;
		mWindowData.Height = properties.Height;

		IG_CORE_INFO("Creating Window: '{}' ({} x {})", properties.Title, properties.Width, properties.Height);

		if (!IsGLFWInitalised) {
			int result = glfwInit();
			IG_CORE_ASSERT(result, "Failed to initalise GLFW!");

			IsGLFWInitalised = true;
		}

		mWindow = glfwCreateWindow((int)properties.Width, (int)properties.Height, properties.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(mWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		IG_CORE_ASSERT(status, "Failed to Initialise GLAD!");
		glfwSetWindowUserPointer(mWindow, &mWindowData);
		SetVSync(true);

		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(
			mWindow,
			[](GLFWwindow* window, int width, int height) {
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				
				windowData.Width = width;
				windowData.Height = height;

				Events::WindowResizeEvent event(width, height);
				windowData.EventCallback(event);
			}
		);

		glfwSetWindowCloseCallback(
			mWindow,
			[](GLFWwindow* window) {
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				Events::WindowCloseEvent event;
				windowData.EventCallback(event);
			}
		);

		glfwSetKeyCallback(
			mWindow,
			[](GLFWwindow* window, int key, int scancode, int action, int modifiers) {
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action) {
					case GLFW_PRESS:
					{
						Events::KeyPressedEvent event(key, 0);
						windowData.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						Events::KeyReleasedEvent event(key);
						windowData.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						Events::KeyPressedEvent event(key, 1);
						windowData.EventCallback(event);
						break;
					}
				}
			}
		);

		glfwSetMouseButtonCallback(
			mWindow,
			[](GLFWwindow* window, int button, int action, int modifiers) {
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
				case GLFW_PRESS:
				{
					Events::MouseButtonPressedEvent event(button);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					Events::MouseButtonReleasedEvent event(button);
					windowData.EventCallback(event);
					break;
				}

				}
			}
		);

		glfwSetScrollCallback(
			mWindow,
			[](GLFWwindow* window, double xOffset, double yOffset) {
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				Events::MouseScrollEvent event((float)xOffset, (float)yOffset);
				windowData.EventCallback(event);
			}
		);

		glfwSetCursorPosCallback(
			mWindow,
			[](GLFWwindow* window, double x, double y) {
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				Events::MouseMovedEvent event((float)x, (float)y);
				windowData.EventCallback(event);
			}
		);
	}

	void Window_Impl_Windows::End() {
		glfwDestroyWindow(mWindow);
	}

	void Window_Impl_Windows::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}

	void Window_Impl_Windows::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		mWindowData.VSync = enabled;
	}

	bool Window_Impl_Windows::IsVSync() const {
		return mWindowData.VSync;
	}
}