#pragma once

#include "Ignition/Events/Event.hpp"

namespace Ignition::Events {
	class IGNITION_API MouseEvent : public Event {
	public:
		MouseEvent(int x, int y)
			: mX(x)
			, mY(y)
		{}

		inline int GetX() const { return mX; }
		inline int GetY() const { return mY; }
		inline std::string GetName() const override { return "MouseEvent"; }
		inline EventType GetType() const override { return EventType::Mouse; }
		inline EventCategory GetCategory() const override { return EventCategory::Input; }

	private:
		int mX, mY;
	};
}