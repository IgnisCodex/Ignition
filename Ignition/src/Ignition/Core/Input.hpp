#pragma once

#include "Ignition/API.hpp"

namespace Ignition::Core {
	class IGNITION_API Input {
	public:
		inline static bool IsKeyPressed(int keycode) { return sInstance->IsKeyPressed_Impl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return sInstance->IsMouseButtonPressed_Impl(button); }
		inline static std::pair<float, float> GetCursorPosition() { return sInstance->GetCursorPosition_Impl(); }
		inline static float GetCursorX() { return sInstance->GetCursorX_Impl(); }
		inline static float GetCursorY() { return sInstance->GetCursorY_Impl(); }
		
		inline static glm::vec2 GetMouseDelta() {
			std::pair currentMousePos = GetCursorPosition();
			static std::pair lastMousePos = currentMousePos;

			std::pair delta = { currentMousePos.first - lastMousePos.first, currentMousePos.second - lastMousePos.second };
			lastMousePos = currentMousePos;

			return glm::vec2(delta.first, delta.second);
		}

	protected:
		virtual bool IsKeyPressed_Impl(int keycode) = 0;

		virtual bool IsMouseButtonPressed_Impl(int button) = 0;
		virtual std::pair<float, float> GetCursorPosition_Impl() = 0;
		virtual float GetCursorX_Impl() = 0;
		virtual float GetCursorY_Impl() = 0;

	private:
		static Input* sInstance;
	};
}