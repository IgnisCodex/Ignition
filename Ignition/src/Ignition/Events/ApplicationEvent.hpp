#pragma once

#include "Ignition/Events/Event.hpp"

namespace Ignition::Events {
	class IGNITION_API ApplicationEvent : public Event {
	public:
		inline EventCategory GetCategory() const override { return EventCategory::Application; }
	};

	class IGNITION_API WindowResizeEvent : public ApplicationEvent {
	public:
		WindowResizeEvent(int width, int height)
			: mWidth(width)
			, mHeight(height)
		{}

		inline std::string GetName() const override { return "WindowResizeEvent"; }
		inline EventType GetType() const override { return EventType::Keyboard; }
		inline int GetWidth() const { return mWidth; }
		inline int GetHeight() const { return mHeight; }

	private:
		int mWidth, mHeight;
	};

	class IGNITION_API WindowCloseEvent : public ApplicationEvent {
		inline std::string GetName() const override { return "WindowCloseEvent"; }
	};
}