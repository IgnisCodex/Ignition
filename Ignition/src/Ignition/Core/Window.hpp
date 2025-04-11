#pragma once

#include "IGPCH.hpp"
#include "Ignition/Events/Event.hpp"

namespace Ignition::Core {
	struct WindowProperties {
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(
			const std::string title = "Ignition",
			uint32_t width = 1280,
			uint32_t height = 720
		)
			: Title(title)
			, Width(width)
			, Height(height)
		{}
	};

	class Window {
	public:
		using EventCallback = std::function<void(Events::Event&)>;

		virtual ~Window() {}

		virtual void* GetNativeWindow() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetEventCallback(const EventCallback& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;

		virtual bool IsVSync() const = 0;

		virtual void OnUpdate() = 0;

		static IGScope<Window> Create(const WindowProperties& properties = WindowProperties());
	};
}