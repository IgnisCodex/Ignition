#pragma once

#include "IGPCH.hpp"
#include "Ignition/API.hpp"
#include "Ignition/Events/Event.hpp"

namespace Ignition::Core {
	class IGNITION_API Layer {
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate() {}
		virtual void OnEvent(Events::Event& event) {}

		inline const std::string& GetDebugName() const { return mDebugName; }
		
	protected:
		std::string mDebugName;
	};
}