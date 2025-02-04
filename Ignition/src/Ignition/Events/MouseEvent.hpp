#pragma once

#include "Ignition/Events/Event.hpp"

namespace Ignition::Events {
	class IGNITION_API MouseEvent : public Event {
	public:
		EVENT_TYPE(EventType::Mouse)
		EVENT_CATEGORY(EventCategory::Input)
	};

	class IGNITION_API MouseMovedEvent : public MouseEvent {
	public:
		MouseMovedEvent(float x, float y)
			: mX(x)
			, mY(y)
		{}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mX << ", " << mY;
			return ss.str();
		}
		inline int GetX() const { return mX; }
		inline int GetY() const { return mY; }

	private:
		float mX, mY;
	};

	class IGNITION_API MouseScrollEvent : public MouseEvent {
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			: mXOffset(xOffset)
			, mYOffset(yOffset)
		{}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "MouseScrollEvent: " << mXOffset << ", " << mYOffset;
			return ss.str();
		}
		inline int GetXOffset() const { return mXOffset; }
		inline int GetYOffset() const { return mYOffset; }

	private:
		float mXOffset, mYOffset;
	};

	class IGNITION_API MouseButtonEvent : public MouseEvent {
	public:
		inline int GetMouseButton() const { return mButton; }

	protected:
		MouseButtonEvent(int button)
			: mButton(button) {
		}

		int mButton;
	};

	class IGNITION_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mButton;
			return ss.str();
		}
	};

	class IGNITION_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mButton;
			return ss.str();
		}
	};
}