#pragma once

#include "IGPCH.hpp"
#include "Ignition/API.hpp"

namespace Ignition::Events {

	enum class EventType {
		None,

		// Window Event Types
		WindowClosed,
		WindowResized,
		WindowMoved,
		WindowFocused,
		WindowUnfocused,

		// Keyboard Event Types
		KeyPressed,
		KeyReleased,

		// Mouse Event Types
		MouseMoved,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseScrolled,

		// Application Event Types
		ApplicationTick,
		ApplicationUpdate,
		ApplicationRender
	};

	enum class EventCategory {
		None,

		Application,
		Input
	};

#define EVENT_TYPE(type) static EventType GetStaticType() { return type; }\
	virtual EventType GetType() const override { return GetStaticType(); }

#define EVENT_CATEGORY(category) virtual EventCategory GetCategory() const override { return category; }

	class Event {
		friend class EventDispatcher;
	public:
		virtual ~Event() = default;
		virtual std::string GetName() const = 0;
		virtual EventType GetType() const = 0;
		virtual EventCategory GetCategory() const = 0;

		bool IsType(EventType type) const {
			return GetType() == type;
		}

		bool IsInCategory(EventCategory category) const {
			return GetCategory() == category;
		}

		bool IsHandled = false;
	};

	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: mEvent(event) 
		{}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (mEvent.GetType() == T::GetStaticType()) {
				mEvent.IsHandled = func(*(T*)&mEvent);
				return true;
			}

			return false;
		}

	private:
		Event& mEvent;
	};
}