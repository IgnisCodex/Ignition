#pragma once

#include "Ignition/Events/Event.hpp"

#include "Ignition/Log.hpp"

namespace Ignition::Events {
	class IGNITION_API ApplicationEvent : public Event {
	public:
		EVENT_CATEGORY(EventCategory::Application)
	};

	class IGNITION_API WindowResizeEvent : public ApplicationEvent {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: mWidth(width)
			, mHeight(height)
		{
			IG_CORE_WARN("{}, {}", width, height);
		}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}
		
		inline unsigned int GetWidth() const { return mWidth; }
		inline unsigned int GetHeight() const { return mHeight; }

		EVENT_TYPE(EventType::WindowResized)

	private:
		unsigned int mWidth, mHeight;
	};

	class IGNITION_API WindowCloseEvent : public ApplicationEvent {
	public:
		WindowCloseEvent() {}

		inline std::string GetName() const override { return "WindowCloseEvent"; }
		EVENT_TYPE(EventType::WindowClosed)
	};
}