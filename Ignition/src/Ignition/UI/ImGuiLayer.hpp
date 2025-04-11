#pragma once

#include "Ignition/Events/Event.hpp"
#include "Ignition/Core/Layer.hpp"

namespace Ignition::UI {
	class ImGuiLayer : public Core::Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void SetStyle();
		void SetDarkTheme();

	private:
		float mTime = 0.0f;
	};
}