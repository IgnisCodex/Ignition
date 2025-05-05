#include "IGPCH.hpp"
#include "Ignition/Scene/Scene.hpp"

#include "Ignition/Graphics/Renderer2D.hpp"
#include "Ignition/Scene/Components.hpp"
#include "Ignition/Scene/Object.hpp"

namespace Ignition::Scene {
	Scene::Scene() {}

	Scene::~Scene() {}

	Object Scene::CreateObject(const std::string& name) {
		Object obj = { mRegistry.create(), this };
		obj.AddComponent<TransformComponent>();
		auto& tag = obj.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Game Object" : name;

		return obj;
	}

	void Scene::DeleteObject(Object obj) {
		mRegistry.destroy(obj);
	}

	void Scene::OnUpdate(Util::DeltaTime dt) {

		{
			mRegistry.view<NativeScriptComponent>().each(
				[=](auto obj, auto& nsc) {
					if (!nsc.Instance) {
						nsc.Instance = nsc.CreateInstance();
						nsc.Instance->mObject = Object{ obj, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(dt);
				});
		}

		Graphics::Camera* activeCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = mRegistry.view<TransformComponent, CameraComponent>();
			for (auto obj : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(obj);

				if (camera.Active) {
					activeCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (activeCamera) {
			Graphics::Renderer2D::SceneBegin(*activeCamera, cameraTransform);

			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto obj : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(obj);

				Graphics::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Colour);
			}

			Graphics::Renderer2D::SceneEnd();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		if (mViewportWidth == width && mViewportHeight == height)
			return;

		mViewportWidth = width;
		mViewportHeight = height;

		// Resize Non FixedAspectRatio cameras
		auto view = mRegistry.view<CameraComponent>();
		for (auto obj : view) {
			auto& cameraComponent = view.get<CameraComponent>(obj);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
}