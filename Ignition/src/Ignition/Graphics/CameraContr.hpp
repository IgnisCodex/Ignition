#pragma once

#include "Ignition/Events/ApplicationEvent.hpp"
#include "Ignition/Events/MouseEvent.hpp"
#include "Ignition/Graphics/Camera.hpp"
#include "Ignition/Util/Time.hpp"

namespace Ignition::Graphics {
	class OrthoCameraContr {
	public:
		OrthoCameraContr(float aspectRatio, bool enableRotation = false);

		void OnUpdate(Util::DeltaTime dt);
		void OnEvent(Events::Event& event);

		OrthoCamera& GetCamera() { return mCamera; }
		const OrthoCamera& GetCamera() const { return mCamera; }

	private:
		bool OnMouseScroll(Events::MouseScrollEvent& event);
		bool OnWindowResize(Events::WindowResizeEvent& event);

	private:
		float mAspectRatio;
		float mZoom = 1.0f;
		bool mEnableRotation;

		OrthoCamera mCamera;

		float mRotation = 0.0f;
		glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
		float mTranslationSpeed = 1.0f;
		float mRotationSpeed = 1.0f;
	};
}