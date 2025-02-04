// Window implementation for Windows

#pragma once

#include "Ignition/Window.hpp"

#include <GLFW/glfw3.h>

namespace Ignition {
	class Window_Impl_Windows : public Window {
	public:
		Window_Impl_Windows(const WindowProperties& properties);
		virtual ~Window_Impl_Windows();

		inline unsigned int GetWidth() const override { return mWindowData.Width; }
		inline unsigned int GetHeight() const override { return mWindowData.Height; }
		inline void SetEventCallback(const EventCallback& callback) override { mWindowData.EventCallback = callback; }
		void SetVSync(bool enabled) override;

		bool IsVSync() const override;

		void OnUpdate() override;

	private:
		virtual void Init(const WindowProperties& properties);
		virtual void End();

	private:
		struct WindowData {
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;
			EventCallback EventCallback;
		};

		GLFWwindow* mWindow;
		WindowData mWindowData;
	};
}