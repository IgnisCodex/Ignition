// Window implementation for Windows

#pragma once

#include "Ignition/Core/Window.hpp"
#include "Ignition/Graphics/Context.hpp"

#include <GLFW/glfw3.h>

namespace Ignition {
	class Window_Impl_Windows : public Core::Window {
	public:
		Window_Impl_Windows(const Core::WindowProperties& properties);
		virtual ~Window_Impl_Windows();

		virtual void* GetNativeWindow() const { return mWindow; }
		inline uint32_t GetWidth() const override { return mWindowData.Width; }
		inline uint32_t GetHeight() const override { return mWindowData.Height; }
		inline void SetEventCallback(const EventCallback& callback) override { mWindowData.EventCallback = callback; }
		void SetVSync(bool enabled) override;

		bool IsVSync() const override;

		void OnUpdate() override;

	private:
		virtual void Init(const Core::WindowProperties& properties);
		virtual void End();

	private:
		struct WindowData {
			std::string Title;
			unsigned int Width = 0;
			unsigned int Height = 0;
			bool VSync = true;
			EventCallback EventCallback;
		};

		GLFWwindow* mWindow;
		WindowData mWindowData;
		Graphics::Context* mContext;
	};
}