#pragma once

#include "Ignition/Events/Event.hpp"

namespace Ignition::Events {
	class IGNITION_API ApplicationEvent : public Event {
	public:
		EVENT_CATEGORY(EventCategory::Application)
	};

	class IGNITION_API WindowResizeEvent : public ApplicationEvent {
	public:
		WindowResizeEvent(int width, int height)
			: mWidth(width)
			, mHeight(height)
		{}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}
		
		inline int GetWidth() const { return mWidth; }
		inline int GetHeight() const { return mHeight; }

		EVENT_TYPE(EventType::Window);

	private:
		int mWidth, mHeight;
	};

	class IGNITION_API WindowCloseEvent : public ApplicationEvent {
	public:
		inline std::string GetName() const override { return "WindowCloseEvent"; }
		EVENT_TYPE(EventType::Window)
	};
}