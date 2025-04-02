#include "IGPCH.hpp"
#include "Ignition/Scene/Scene.hpp"

#include "Ignition/Graphics/Renderer2D.hpp"
#include "Ignition/Scene/Components.hpp"
#include "Ignition/Scene/GameObject.hpp"

namespace Ignition::Scene {
	Scene::Scene() {
		entt::entity go = mRegistry.create();
	}

	Scene::~Scene() {}

	GameObject Scene::CreateGameObject(const std::string& name) {
		GameObject go = { mRegistry.create(), this };
		go.AddComponent<TransformComponent>();
		auto& tag = go.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Game Object" : name;

		return go;
	}

	void Scene::OnUpdate(Util::DeltaTime dt) {

		Graphics::Camera* activeCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = mRegistry.view<TransformComponent, CameraComponent>();
			for (auto go : view) {
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(go);

				if (camera.Active) {
					activeCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (activeCamera) {
			Graphics::Renderer2D::SceneBegin(activeCamera->GetProjection(), *cameraTransform);

			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto go : group) {
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(go);

				Graphics::Renderer2D::DrawQuad(transform, sprite.Colour);
			}

			Graphics::Renderer2D::SceneEnd();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		mViewportWidth = width;
		mViewportHeight = height;

		auto view = mRegistry.view<CameraComponent>();
		for (auto go : view) {
			auto& cameraComponent = view.get<CameraComponent>(go);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}
}