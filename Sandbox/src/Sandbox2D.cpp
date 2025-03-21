#include "Sandbox2D.hpp"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
	, mCameraContr(1280.0f / 720.0f)
{}

void Sandbox2D::OnAttach() {
	mTexture = Ignition::Graphics::Texture2D::Create("assets/textures/band.png");
	//6 * 4
	mTileSheet = Ignition::Graphics::Texture2D::Create("assets/textures/tsHome.png");
	mFlower = Ignition::Graphics::SubTexture2D::Create(mTileSheet, glm::vec2(0, 9), glm::vec2(20, 20));
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Ignition::Util::DeltaTime dt) {

	Ignition::Graphics::RenderCall::Clear(rgb(49, 51, 59));

	mCameraContr.OnUpdate(dt);

	Ignition::Graphics::Renderer2D::SceneBegin(mCameraContr.GetCamera());
		
	//Ignition::Graphics::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rgb(255, 255, 0));
	//Ignition::Graphics::Renderer2D::DrawQuad({ 0.0f, 1.5f }, { 1.0f, 1.0f }, rgb(255, 0, 255));
	Ignition::Graphics::Renderer2D::DrawQuad({ 1.5f, 0.0f }, { 1.0f, 1.0f }, mTileSheet);

	IG_INFO("mTilesheet: w= {} y= {}", mTileSheet->GetWidth(), mTileSheet->GetHeight());
	Ignition::Graphics::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, mFlower);

	Ignition::Graphics::Renderer2D::SceneEnd();
}

void Sandbox2D::OnImGuiRender() {
	ImGui::ShowDemoWindow();
}

void Sandbox2D::OnEvent(Ignition::Events::Event& event) {
	mCameraContr.OnEvent(event);
}