#pragma once

#include <Ignition.hpp>

class Sandbox2D : public Ignition::Core::Layer {
public:
	Sandbox2D();
	~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Ignition::Util::DeltaTime dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Ignition::Events::Event& event) override;

private:
	Ignition::Graphics::OrthoCameraContr mCameraContr;

	IGRef<Ignition::Graphics::Shader> mFlatColourShader;
	IGRef<Ignition::Graphics::Texture2D> mTexture;
	IGRef<Ignition::Graphics::Framebuffer> mFramebuffer;

	IGRef<Ignition::Graphics::Texture2D> mTileSheet;
	IGRef<Ignition::Graphics::SubTexture2D> mFlower;

	IGRef<Ignition::Graphics::VertexArray> mSquareVA;

	Ignition::Debug::BlackBox mBlackBox;

	IGColour mSquareColour = rgb(30, 30, 100);
	IGColour mTint = rgb(0, 0, 255);
};