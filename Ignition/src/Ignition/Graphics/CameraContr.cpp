#include "IGPCH.hpp"
#include "Ignition/Graphics/CameraContr.hpp"

#include "Ignition/Core/Input.hpp"
#include "Ignition/Core/InputCodes.hpp"

namespace Ignition::Graphics {
	OrthoCameraContr::OrthoCameraContr(float aspectRatio, bool enableRotation)
		: mAspectRatio(aspectRatio)
		, mCamera(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom)
		, mEnableRotation(enableRotation)
	{}

	void OrthoCameraContr::OnUpdate(Util::DeltaTime dt) {
		// TODO: Make this a mouse control not a keyboard control

		if (Core::Input::IsKeyPressed(IG_KEY_W))
			mPosition.y += mTranslationSpeed * dt.s();

		else if (Core::Input::IsKeyPressed(IG_KEY_S))
			mPosition.y -= mTranslationSpeed * dt.s();
		

		if (Core::Input::IsKeyPressed(IG_KEY_D))
			mPosition.x += mTranslationSpeed * dt.s();

		else if (Core::Input::IsKeyPressed(IG_KEY_A))
			mPosition.x -= mTranslationSpeed * dt.s();

		if (mEnableRotation) {
			if (Core::Input::IsKeyPressed(IG_KEY_Q))
				mRotation += mRotationSpeed * dt.s();

			else if (Core::Input::IsKeyPressed(IG_KEY_A))
				mRotation -= mRotationSpeed * dt.s();

			mCamera.SetRotation(mRotation);
		}

		mCamera.SetPosition(mPosition);
		mTranslationSpeed = mZoom;
	}

	void OrthoCameraContr::OnEvent(Events::Event& event) {
		Events::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Events::MouseScrollEvent>(IG_BIND_EVENT(OrthoCameraContr::OnMouseScroll));
		dispatcher.Dispatch<Events::WindowResizeEvent>(IG_BIND_EVENT(OrthoCameraContr::OnWindowResize));
	}

	void OrthoCameraContr::Resize(float width, float height) {
		mAspectRatio = width / height;
		mCamera.SetProjectionMatrix(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom);
	}

	bool OrthoCameraContr::OnMouseScroll(Events::MouseScrollEvent& event) {
		mZoom -= event.GetYOffset() * 0.25f;
		mZoom = std::max(mZoom, 0.25f);
		mCamera.SetProjectionMatrix(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom);
		return false;
	}

	bool OrthoCameraContr::OnWindowResize(Events::WindowResizeEvent& event) {
		Resize((float)event.GetWidth(), (float)event.GetHeight());
		return false;
	}
}