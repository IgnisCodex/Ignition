#include "SceneTree.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Ignition::UI {
	SceneTree::SceneTree(const IGRef<Scene::Scene>& context) {
		SetContext(context);
	}

	void SceneTree::SetContext(const IGRef<Scene::Scene>& context) {
		mSceneContext = context;
	}

	void SceneTree::OnImGuiRender() {
		if (ImGui::Begin("Scene")) {

			// Right click context menu for individual objects
			ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
			if (ImGui::BeginPopupContextWindow(0, flags)) {

				if (ImGui::BeginMenu("New")) {
					if (ImGui::MenuItem("Empty Object"))
						mSelectedGameObjectContext = mSceneContext->CreateGameObject("Unnamed Object");

					// -------- Custom -------
					if (ImGui::MenuItem("Quad")) {
						auto& go = mSceneContext->CreateGameObject("Unnamed Quad");
						go.AddComponent<Scene::SpriteRendererComponent>();
						mSelectedGameObjectContext = go;
					}

					if (ImGui::MenuItem("Camera")) {
						auto& go = mSceneContext->CreateGameObject("Unnamed Camera");
						go.AddComponent<Scene::CameraComponent>();
						go.GetComponent<Scene::CameraComponent>().Active = false;
						mSelectedGameObjectContext = go;
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}

			auto view = mSceneContext->GetRegistry().view<entt::entity>();
			view.each([this](auto goID) {
				Scene::GameObject go{ goID, mSceneContext.get() };
				DrawTree(go);
			});

			// If left clicked on window, set context to nothing
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				mSelectedGameObjectContext = {};
		}

		ImGui::End();


		if (ImGui::Begin("Properties")) {
			if (mSelectedGameObjectContext) {
				DrawComponents(mSelectedGameObjectContext);

				// Right click context menu for the object's property options
				ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
				if (ImGui::BeginPopupContextWindow(0, flags)) {

					if (ImGui::BeginMenu("Add Component")) {
						if (ImGui::MenuItem("Sprite Renderer"))
							mSelectedGameObjectContext.AddComponent<Scene::SpriteRendererComponent>();

						ImGui::EndMenu();
					}

					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();
	}

	void SceneTree::DrawTree(Scene::GameObject go) {
		if (go.HasComponent<Scene::TagComponent>()) {
			auto& tag = go.GetComponent<Scene::TagComponent>().Tag;

			ImGuiTreeNodeFlags flags =
				ImGuiTreeNodeFlags_OpenOnArrow |
				((mSelectedGameObjectContext == go) ? ImGuiTreeNodeFlags_Selected : 0);
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)go, flags, tag.c_str())) {
				ImGui::TreePop();
			}

			if (ImGui::IsItemClicked()) {
				mSelectedGameObjectContext = go;
			}

			bool goDeleted = false;
			if (ImGui::BeginPopupContextItem()) {

				if (ImGui::MenuItem("Focus on Selected", "", false, false));

				ImGui::Separator();

				glm::vec4 red = rgb(230, 100, 115);
				glm::vec4 white = rgb(255, 255, 255);

				ImGui::PushStyleColor(ImGuiCol_Text, { red.r, red.g, red.b, red.a });
				if (ImGui::MenuItem("Delete"))
					goDeleted = true;
				ImGui::PopStyleColor();

				ImGui::EndPopup();
			}


			if (go.HasComponent<Scene::TagComponent>()) {
				auto& tc = go.GetComponent<Scene::TagComponent>();

			}

			if (goDeleted) {
				mSceneContext->DeleteGameObject(go);
				if (mSelectedGameObjectContext == go) {
					mSelectedGameObjectContext = {};
				}
			}
		}
	}

	static void DragFloat3(const std::string& label, glm::vec3& values, float reset = 0.0f, float columnWidth = 100.0f) {
		ImGui::PushID(label.c_str());
		
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 });
		
		float height = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { height + 3.0f, height };
		
		glm::vec4 red = rgb(230, 100, 115);
		glm::vec4 redHovered = rgb(250, 130, 140);

		glm::vec4 green = rgb(100, 230, 140);
		glm::vec4 greenHovered = rgb(130, 250, 170);

		glm::vec4 blue = rgb(100, 170, 240);
		glm::vec4 blueHovered = rgb(130, 200, 255);

		ImGui::PushStyleColor(ImGuiCol_Button, { red.r, red.g, red.b, red.a });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { red.r, red.g, red.b, red.a });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { redHovered.r, redHovered.g, redHovered.b, redHovered.a });

		if (ImGui::Button("X", buttonSize))
			values.x = reset;
		ImGui::SameLine();

		ImGui::PopStyleColor(3);

		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { green.r, green.g, green.b, green.a });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { green.r, green.g, green.b, green.a });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { greenHovered.r, greenHovered.g, greenHovered.b, greenHovered.a });
		if (ImGui::Button("Y", buttonSize))
			values.y = reset;
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { blue.r, blue.g, blue.b, blue.a });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { blue.r, blue.g, blue.b, blue.a });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { blueHovered.r, blueHovered.g, blueHovered.b, blueHovered.a });
		if (ImGui::Button("Z", buttonSize))
			values.z = reset;
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}
	static void ColourEdit4(const std::string& label, glm::vec4& colour, float columnWidth = 100.0f) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::ColorEdit4("##Colour", glm::value_ptr(colour));
		ImGui::PopID();
	}

	template<typename T, typename UIFunc>
	static void DrawComponent(const std::string& label, Scene::GameObject go, UIFunc uiFunc) {

		if (go.HasComponent<T>()) {

			const ImGuiTreeNodeFlags flags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_Framed;
			auto& component = go.GetComponent<T>();
			if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label.c_str())) {

				// FIXME: Weird Bug
				if (ImGui::BeginPopupContextItem()) {

					//ImGui::Separator();

					glm::vec4 red = rgb(230, 100, 115);

					ImGui::PushStyleColor(ImGuiCol_Text, { red.r, red.g, red.b, red.a });
					if (ImGui::MenuItem("Remove"));
					ImGui::PopStyleColor();

					ImGui::EndPopup();
				}

				uiFunc(component);
				ImGui::TreePop();
			}

			

		}
	}

	void SceneTree::DrawComponents(Scene::GameObject go) {
		if (go.HasComponent<Scene::TagComponent>()) {
			auto& tag = go.GetComponent<Scene::TagComponent>().Tag;
			char buffer[256]{};
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
 		}

		DrawComponent<Scene::TransformComponent>("Transform", go, [](auto& component) {
			DragFloat3("Translation", component.Translation);

			glm::vec3 rotation = glm::degrees(component.Rotation);
			DragFloat3("Rotation", rotation);
			component.Rotation = glm::radians(rotation);

			DragFloat3("Scale", component.Scale, 1.0f);
		});

		DrawComponent<Scene::CameraComponent>("Camera", go, [](auto& compontent) {

			auto& camera = compontent.Camera;

			ImGui::Checkbox("Active", &compontent.Active);

			const char* list[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeStr = list[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeStr)) {
				for (int i = 0; i < 2; i++) {
					bool selected = currentProjectionTypeStr == list[i];
					if (ImGui::Selectable(list[i], selected)) {
						currentProjectionTypeStr = list[i];
						camera.SetProjectionType((Scene::SceneCamera::ProjectionType)i);
					}

					if (selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == Scene::SceneCamera::ProjectionType::Perspective) {
				float perspVFOV = glm::degrees(camera.GetPerspVFOV());
				float perspNearClip = camera.GetOrthoNearClip();
				float perspFarClip = camera.GetOrthoFarClip();

				if (ImGui::DragFloat("Vertical FOV", &perspVFOV))
					camera.SetPerspVFOV(glm::radians(perspVFOV));

				if (ImGui::DragFloat("Near Clip", &perspNearClip))
					camera.SetPerspNearClip(perspNearClip);

				if (ImGui::DragFloat("Far Clip", &perspFarClip))
					camera.SetPerspFarClip(perspFarClip);
			}

			if (camera.GetProjectionType() == Scene::SceneCamera::ProjectionType::Orthographic) {
				float orthoSize = camera.GetOrthoSize();
				float orthoNearClip = camera.GetOrthoNearClip();
				float orthoFarClip = camera.GetOrthoFarClip();

				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthoSize(orthoSize);

				if (ImGui::DragFloat("Near Clip", &orthoNearClip))
					camera.SetOrthoNearClip(orthoNearClip);

				if (ImGui::DragFloat("Far Clip", &orthoFarClip))
					camera.SetOrthoFarClip(orthoFarClip);

				ImGui::Checkbox("Fixed Aspect Ratio", &compontent.FixedAspectRatio);
			}
		});

		DrawComponent<Scene::SpriteRendererComponent>("Sprite", go, [](auto& compontent) {
			ColourEdit4("Colour", compontent.Colour);
		});

	}

	
}