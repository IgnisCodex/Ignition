#pragma once

#include "IGPCH.hpp"
#include "Ignition/Events/Event.hpp"

namespace Ignition {
	struct WindowProperties {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(
			const std::string title = "Ignition",
			unsigned int width = 1280,
			unsigned int height = 720
		)
			: Title(title)
			, Width(width)
			, Height(height)
		{}
	};

	class IGNITION_API Window {
	public:
		using EventCallback = std::function<void(Events::Event&)>;

		virtual ~Window() {}

		virtual void* GetNativeWindow() const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void SetEventCallback(const EventCallback& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;

		virtual bool IsVSync() const = 0;

		virtual void OnUpdate() = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}