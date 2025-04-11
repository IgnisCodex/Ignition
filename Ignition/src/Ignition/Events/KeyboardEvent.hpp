#pragma once

#include "IGPCH.hpp"
#include "Ignition/Events/Event.hpp"

namespace Ignition::Events {
	class KeyboardEvent : public Event {
	public:
		inline int GetKeyCode() const { return mKeyCode; }
		EVENT_CATEGORY(EventCategory::Input)

	protected:
		KeyboardEvent(int key)
			: mKeyCode(key) {
		}

		int mKeyCode;
	};

	class KeyPressedEvent : public KeyboardEvent {
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

		EVENT_TYPE(EventType::KeyPressed)

	private:
		int mRepeatedKeyCount;
	};

	class KeyReleasedEvent : public KeyboardEvent {
	public:
		KeyReleasedEvent(int key)
			: KeyboardEvent(key)
		{}

		inline std::string GetName() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: Key '" << mKeyCode << "'";
			return ss.str();
		}

		EVENT_TYPE(EventType::KeyReleased)
	};
}