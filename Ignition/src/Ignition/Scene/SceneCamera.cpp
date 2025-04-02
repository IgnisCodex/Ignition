#include "IGPCH.hpp"
#include "Ignition/Scene/SceneCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ignition::Scene {
	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetOrtho(float size, float nearClip, float farClip) {
		mOrthographicSize = size;
		mOrthographicNear = nearClip;
		mOrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		mAspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection() {
		float left = -mOrthographicSize * mAspectRatio * 0.5f;
		float right = mOrthographicSize * mAspectRatio * 0.5f;
		float bottom = -mOrthographicSize * 0.5f;
		float top = mOrthographicSize * 0.5f;

		mProjection = glm::ortho(left, right, bottom, top, mOrthographicNear, mOrthographicFar);
	}
}