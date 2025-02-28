#include "IGPCH.hpp"
#include "Ignition/Graphics/Camera.hpp"

#include "Ignition/Log.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ignition::Graphics {

	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
		: mProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), mViewMatrix(1.0f)
	{
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void OrthoCamera::RecalculateViewMatrix() {
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, mPosition);
		transform = glm::rotate(transform, glm::radians(mRotation), glm::vec3(0, 0, 1));
		mViewMatrix = glm::inverse(transform);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}
}