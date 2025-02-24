#pragma once

#include "Ignition/Graphics/Context.hpp"

struct GLFWwindow;

namespace Ignition {
	class OpenGLContext : public Graphics::Context {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* mWindowHandle;
	};
}   