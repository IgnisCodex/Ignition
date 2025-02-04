#pragma once

#include "IGPCH.hpp"
#include "Ignition/API.hpp"

namespace Ignition::Events {

	enum class EventType {
		None,

		Window,
		Keyboard,
		Mouse
	};

	enum class EventCategory {
		None,

		Application,
		Input
	};

	class IGNITION_API Event {
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
	};

	class EventDispatcher {
	public:
		using EventCallback = std::function<void(const Event&)>;

		template <typename T>
		void Register(std::function<void(const T&)> callback) {
			auto wrapper = [callback](const Event& event) {
				callback(static_cast<const T&>(event));
			};

			mListeners[T::StaticType()].push_back(wrapper);
		}

		void Dispatch(const Event& event) {
			auto it = mListeners.find(event.GetName());
			if (it != mListeners.end()) {
				for (auto& listener : it->second) {
					listener(event);
				}
			}
		}

	private:
		std::unordered_map<std::string, std::vector<EventCallback>> mListeners;
	};
}