#include "Sandbox2D.hpp"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
	, mCameraContr(1280.0f / 720.0f)
{}

void Sandbox2D::OnAttach() {
	mTexture = Ignition::Graphics::Texture2D::Create("assets/textures/band.png");
}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(Ignition::Util::DeltaTime dt) {

	Ignition::Graphics::RenderCall::Clear(rgb(15, 23, 42));

	mCameraContr.OnUpdate(dt);
	
	Ignition::Graphics::Renderer2D::SceneBegin(mCameraContr.GetCamera());

	Ignition::Graphics::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rgba(255, 80, 80, 1.0f));
	Ignition::Graphics::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, mSquareColour);
	Ignition::Graphics::Renderer2D::DrawQuad({ 0.0f, 1.0f }, { 1.0f, 1.0f }, mTexture);

	Ignition::Graphics::Renderer2D::SceneEnd();
}

void Sandbox2D::OnImGuiRender() {
	if (ImGui::Begin("Settings")) {
		ImGui::ColorEdit4("Square Colour", glm::value_ptr(mSquareColour));
		ImGui::End();
	}
}

void Sandbox2D::OnEvent(Ignition::Events::Event& event) {
	mCameraContr.OnEvent(event);
}