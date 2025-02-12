#include "IGPCH.hpp"

#include "Backends/OpenGL/OpenGLContext.hpp"

#include "Ignition/Log.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Ignition {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: mWindowHandle(windowHandle)
	{
		IG_CORE_ASSERT(windowHandle, "windowHandle is NULL!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(mWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		IG_CORE_ASSERT(status, "Failed to Initalise GLAD!");
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(mWindowHandle);
	}
}