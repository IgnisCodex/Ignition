#pragma once

#include "IGPCH.hpp"

#include "API.hpp"

namespace Ignition {
	class IGNITION_API Input {
	public:
		inline static bool IsKeyPressed(int keycode) { return sInstance->IsKeyPressed_Impl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return sInstance->IsMouseButtonPressed_Impl(button); }
		inline static std::pair<float, float> GetCursorPosition() { return sInstance->GetCursorPosition_Impl(); }
		inline static float GetCursorX() { return sInstance->GetCursorX_Impl(); }
		inline static float GetCursorY() { return sInstance->GetCursorY_Impl(); }

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