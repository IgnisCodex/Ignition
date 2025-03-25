#include "EditorLayer.hpp"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Ignition {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
		, mCameraContr(1280.0f / 720.0f) {
	}

	void EditorLayer::OnAttach() {
		mTexture = Ignition::Graphics::Texture2D::Create("assets/textures/band.png");
		//6 * 4
		mTileSheet = Ignition::Graphics::Texture2D::Create("assets/textures/tsHome.png");
		mFlower = Ignition::Graphics::SubTexture2D::Create(mTileSheet, glm::vec2(3, 9), glm::vec2(20, 20));

		Ignition::Graphics::FramebufferProperties fbProperties;
		fbProperties.Width = 1280.0f;
		fbProperties.Height = 720.0f;
		mFramebuffer = Ignition::Graphics::Framebuffer::Create(fbProperties);

	}

	void EditorLayer::OnDetach() {

	}

	void EditorLayer::OnUpdate(Ignition::Util::DeltaTime dt) {

		mCameraContr.OnUpdate(dt);

		mFramebuffer->Bind();
		Ignition::Graphics::RenderCall::Clear(rgb(49, 51, 59));
		Ignition::Graphics::Renderer2D::SceneBegin(mCameraContr.GetCamera());

		//Ignition::Graphics::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rgb(255, 255, 0));
		//Ignition::Graphics::Renderer2D::DrawQuad({ 0.0f, 1.5f }, { 1.0f, 1.0f }, rgb(255, 0, 255));
		//Ignition::Graphics::Renderer2D::DrawQuad({ 1.5f, 0.0f }, { 1.0f, 1.0f }, mTileSheet);
		Ignition::Graphics::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, mFlower);

		Ignition::Graphics::Renderer2D::SceneEnd();
		mFramebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender() {

		// Dockspace Setup
		static bool is_open = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("DockSpace", &is_open, window_flags);
		ImGui::PopStyleVar(3);

		// DockSpace Submission
		ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), dockspace_flags);

		// Menu Bar
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Quit"))	Ignition::Core::Application::Get().Quit();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();




		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		if (ImGui::Begin("Viewport"), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse) {

			
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();

			if (mViewportSize != *((glm::vec2*)&viewportSize)) {
				mFramebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				mViewportSize = { viewportSize.x, viewportSize.y };
				
				mCameraContr.Resize(viewportSize.x, viewportSize.y);
			}

			uint32_t textureID = mFramebuffer->GetColourAttachmentRendererID();
			ImGui::Image((ImTextureID)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }/*, ImVec2{0, 1}, ImVec2{1, 0}*/ );

		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::ShowDemoWindow();
	}

	void EditorLayer::OnEvent(Ignition::Events::Event& event) {
		mCameraContr.OnEvent(event);
	}
}