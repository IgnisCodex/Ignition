#include "Platform/Windows/Window_Impl_Windows.hpp"
#include "Ignition/Log.hpp"

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
		glfwSetWindowUserPointer(mWindow, &mWindowData);
		SetVSync(true);
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