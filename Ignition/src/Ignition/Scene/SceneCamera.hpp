#pragma once

#include "Ignition/Graphics/Camera.hpp"

namespace Ignition::Scene {
	class SceneCamera : public Graphics::Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;


		// +---- Getters ----+

		float GetOrthoSize() const { return mOrthoSize; }
		float GetOrthoNearClip() const { return mOrthoNear; }
		float GetOrthoFarClip() const { return mOrthoFar; }

		float GetPerspVFOV() const { return mPerspFOV; }
		float GetPerspNearClip() const { return mPerspNear; }
		float GetPerspFarClip() const { return mPerspFar; }

		ProjectionType GetProjectionType() const { return mProjectionType; }
		

		// +---- Setters ----+

		void SetOrtho(float size, float nearClip, float farClip);
		void SetOrthoSize(float size) { mOrthoSize = size; RecalculateProjection(); };
		void SetOrthoNearClip(float nearClip) { mOrthoNear = nearClip; RecalculateProjection(); }
		void SetOrthoFarClip(float farClip) { mOrthoFar = farClip; RecalculateProjection();}

		void SetPersp(float vfov, float nearClip, float farClip);
		void SetPerspVFOV(float vfov) { mPerspFOV = vfov; RecalculateProjection(); }
		void SetPerspNearClip(float nearClip) { mPerspNear = nearClip; RecalculateProjection(); }
		void SetPerspFarClip(float farClip) { mPerspNear = farClip; RecalculateProjection(); }

		void SetViewportSize(uint32_t width, uint32_t height);
		void SetProjectionType(ProjectionType type) { mProjectionType = type; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType mProjectionType = ProjectionType::Orthographic;
		
		float mOrthoSize = 10.0f;
		float mOrthoNear = -1.0f;
		float mOrthoFar = 1.0f;

		float mPerspFOV = glm::radians(45.0f);
		float mPerspNear = 0.01f;
		float mPerspFar = 1000.0f;

		float mAspectRatio = 0.0f;
	};
}