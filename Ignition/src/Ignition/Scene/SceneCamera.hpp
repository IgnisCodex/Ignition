#pragma once

#include "Ignition/Graphics/Camera.hpp"

namespace Ignition::Scene {
	class SceneCamera : public Graphics::Camera {
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrtho(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		void RecalculateProjection();

	private:
		float mOrthographicSize = 10.0f;
		float mOrthographicNear = -1.0f, mOrthographicFar = 1.0f;

		float mAspectRatio = 0.0f;
	};
}