#pragma once

#include "Ignition/API.hpp"
#include "Ignition/Scene/SceneCamera.hpp"
#include "Ignition/Scene/Object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Ignition::Scene {
	
	struct SpriteRendererComponent {
		glm::vec4 Colour = rgb(255, 255, 255);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& colour)
			: Colour(colour)
		{}
	};

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{}
	};

	struct TransformComponent {
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation)
		{}

		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0 ,0 ,1 });

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale) ;
		}
	};

	struct CameraComponent {
		SceneCamera Camera;											// Scene Camera
		bool Active = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent {
		ScriptableObject* Instance = nullptr;

		ScriptableObject*(*CreateInstance)();
		void (*DestroyInstance)(NativeScriptComponent*);

		template<typename T>
		void Bind() {
			CreateInstance = []() { return static_cast<ScriptableObject*>(new T()); };
			DestroyInstance = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}