#include "IGPCH.hpp"
#include "Ignition/Scene/SceneCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ignition::Scene {
	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetOrtho(float size, float nearClip, float farClip) {
		mProjectionType = ProjectionType::Orthographic;
		
		mOrthoSize = size;
		mOrthoNear = nearClip;
		mOrthoFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPersp(float vfov, float nearClip, float farClip) {
		mProjectionType = ProjectionType::Perspective;

		mPerspFOV = vfov;
		mPerspNear = nearClip;
		mPerspFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		mAspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection() {
		if (mProjectionType == ProjectionType::Perspective) {
			mProjection = glm::perspective(mPerspFOV, mAspectRatio, mPerspNear, mPerspFar);

		} else {
			float left = -mOrthoSize * mAspectRatio * 0.5f;
			float right = mOrthoSize * mAspectRatio * 0.5f;
			float bottom = -mOrthoSize * 0.5f;
			float top = mOrthoSize * 0.5f;

			mProjection = glm::ortho(left, right, bottom, top, mOrthoNear, mOrthoFar);
		}
	}
}