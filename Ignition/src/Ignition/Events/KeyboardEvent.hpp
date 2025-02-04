#pragma once

#include "IGPCH.hpp"
#include "Ignition/Events/Event.hpp"

namespace Ignition::Events {
	class IGNITION_API KeyboardEvent : public Event {
	public:
		inline int GetKeyCode() const { return mKeyCode; }
		EVENT_TYPE(EventType::Keyboard)
		EVENT_CATEGORY(EventCategory::Input)

	protected:
		KeyboardEvent(int key)
			: mKeyCode(key) {
		}

		int mKeyCode;
	};

	class IGNITION_API KeyPressedEvent : public KeyboardEvent {
	public:
		KeyPressedEvent(int key, int repeat)
			: KeyboardEvent(key)
			, mRepeatedKeyCount(repeat)
		{}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: Key '" << mKeyCode << "'\tRepeat Count '" << mRepeatedKeyCount << "'";
			return ss.str();
		}
		inline int GetRepeatedKeyCount() const { return mRepeatedKeyCount; }

	private:
		int mRepeatedKeyCount;
	};

	class IGNITION_API KeyReleasedEvent : public KeyboardEvent {
	public:
		KeyReleasedEvent(int key)
			: KeyboardEvent(key)
		{}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: Key '" << mKeyCode << "'";
			return ss.str();
		}
	};
}