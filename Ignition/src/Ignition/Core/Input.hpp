#pragma once

#include "Ignition/API.hpp"

namespace Ignition::Core {
	class Input {
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);

		static std::pair<float, float> GetCursorPosition();
		static float GetCursorX();
		static float GetCursorY();
		//static glm::vec2 GetMouseDelta();
	};
}