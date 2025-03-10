#pragma once

#include <Ignition.hpp>
#include <Ignition/Util/Util.hpp>

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

	Ignition::Ref<Ignition::Graphics::Shader> mFlatColourShader;

	Ignition::Ref<Ignition::Graphics::VertexArray> mSquareVA;
	IGColour mSquareColour = rgb(30, 30, 100);
};