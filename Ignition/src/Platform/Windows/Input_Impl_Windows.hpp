#pragma once

#include "IGPCH.hpp"
#include "Ignition/Input.hpp"

namespace Ignition {
	class Input_Impl_Windows : public Input {
	protected:
		virtual bool IsKeyPressed_Impl(int keycode) override;
		virtual bool IsMouseButtonPressed_Impl(int button) override;
		virtual std::pair<float, float> GetCursorPosition_Impl() override;
		virtual float GetCursorY_Impl() override;
		virtual float GetCursorX_Impl() override;
	};
}