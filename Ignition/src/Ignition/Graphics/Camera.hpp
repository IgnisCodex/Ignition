#pragma once

#include <glm/glm.hpp>

namespace Ignition::Graphics {

	class OrthoCamera {
	public:
		OrthoCamera(float left, float right, float bottom, float top);

		const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return mViewProjectionMatrix; }

		const glm::vec3& GetPosition() const { return mPosition; }
		float GetRotation() const { return mRotation; }

		void SetProjectionMatrix(float left, float right, float bottom, float top);
		inline void SetPosition(const glm::vec3& position) { mPosition = position; RecalculateViewMatrix(); }
		inline void SetRotation(float rotation) { mRotation = rotation; RecalculateViewMatrix(); }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewProjectionMatrix;

		glm::vec3 mPosition;
		float mRotation = 0.0f;
	};
}