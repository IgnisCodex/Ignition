#include "EditorLayer.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Ignition {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
		, mCameraContr(1280.0f / 720.0f) {
	}

	void EditorLayer::OnAttach() {
		mTexture = Graphics::Texture2D::Create("assets/textures/band.png");
		//6 * 4
		mTileSheet = Graphics::Texture2D::Create("assets/textures/tsHome.png");
		mFlower = Graphics::SubTexture2D::Create(mTileSheet, glm::vec2(6, 6), glm::vec2(20, 20));

		Graphics::FramebufferProperties fbProperties;
		fbProperties.Width = 1280;
		fbProperties.Height = 720;
		mFramebuffer = Graphics::Framebuffer::Create(fbProperties);

		mActiveScene = IGCreateRef<Scene::Scene>();

		mSceneTree.SetContext(mActiveScene);
	}

	void EditorLayer::OnDetach() {

	}

	void EditorLayer::OnUpdate(Util::DeltaTime dt) {
		
		mActiveScene->OnViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);

		if (Graphics::FramebufferProperties properties = mFramebuffer->GetProperties();
			mViewportSize.x > 0.0f && 
			mViewportSize.y > 0.0f && (
				properties.Width != mViewportSize.x ||
				properties.Height != mViewportSize.y ))
		{
			mFramebuffer->Resize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
			mCameraContr.Resize(mViewportSize.x, mViewportSize.y);
		}
		
		// Updates
		//mCameraContr.OnUpdate(dt); 

		mFramebuffer->Bind();
		Graphics::RenderCall::Clear(rgb(25, 30, 32));

		mActiveScene->OnUpdate(dt);

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
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					mActiveScene = IGCreateRef<Scene::Scene>();
					mActiveScene->OnViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
					mSceneTree.SetContext(mActiveScene);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					std::string filepath = Util::FileDialog::OpenFile("Ignition Scene (*.igscene)\0*.igscene\0");
					if (!filepath.empty()) {
						mActiveScene = IGCreateRef<Scene::Scene>();
						mSceneTree.SetContext(mActiveScene);

						Scene::Serialiser serialiser(mActiveScene);
						serialiser.DeserialiseYAML(filepath);

						mActiveScene->OnViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
					}
				}

				if (ImGui::BeginMenu("Open Recent", false));

				ImGui::Separator();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					std::string filepath = Util::FileDialog::SaveFile("Ignition Scene (*.igscene)\0*.igscene\0");
					if (!filepath.empty()) {
						Scene::Serialiser serialiser(mActiveScene);
						serialiser.SerialiseYAML(filepath);
					}
				}
				
				ImGui::Separator();

				if (ImGui::MenuItem("Quit"))	Core::Application::Get().Quit();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		if (ImGui::Begin("Viewport"), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse) {

			
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			mViewportSize = { viewportSize.x, viewportSize.y };

			uint32_t textureID = mFramebuffer->GetColourAttachmentRendererID();
			ImGui::Image((ImTextureID)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{0, 1}, ImVec2{1, 0} );

		ImGui::End();
		}
		ImGui::PopStyleVar();

		ImGui::ShowDemoWindow();

		mSceneTree.OnImGuiRender();
	}

	void EditorLayer::OnEvent(Events::Event& event) {
		mCameraContr.OnEvent(event);

		Events::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Events::KeyPressedEvent>(IG_BIND_EVENT(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(Events::KeyPressedEvent& event) {
		//switch (event.GetKeyCode())

		return false;
	}


}