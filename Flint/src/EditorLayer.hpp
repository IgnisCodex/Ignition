#pragma once

#include <Ignition.hpp>

namespace Ignition {
	class EditorLayer : public Ignition::Core::Layer {
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Util::DeltaTime dt) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Events::Event& event) override;

	private:
		Graphics::OrthoCameraContr mCameraContr;

		IGRef<Graphics::Shader> mFlatColourShader;
		IGRef<Graphics::Texture2D> mTexture;
		IGRef<Graphics::Framebuffer> mFramebuffer;

		IGRef<Graphics::Texture2D> mTileSheet;
		IGRef<Graphics::SubTexture2D> mFlower;

		IGRef<Graphics::VertexArray> mSquareVA;

		glm::vec2 mViewportSize = { 0.0f, 0.0f };

		IGColour mSquareColour = rgb(30, 30, 100);
		IGColour mTint = rgb(0, 0, 255);
	};
}