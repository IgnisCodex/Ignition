#pragma once

#include "Ignition/Events/Event.hpp"

namespace Ignition::Events {
	class IGNITION_API KeybaordEvent : public Event {
	public:
		KeybaordEvent(int key)
			: mKeyCode(key)
		{}

		inline int GetScanCode() const { return mKeyCode; }
		inline std::string GetName() const override { return "KeyboardEvent"; }
		inline EventType GetType() const override { return EventType::Keyboard; }
		inline EventCategory GetCategory() const override { return EventCategory::Input; }

	private:
		int mKeyCode;
	};
}