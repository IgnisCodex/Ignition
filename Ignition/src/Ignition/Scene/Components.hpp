#pragma once

#include "Ignition/API.hpp"
#include "Ignition/Scene/SceneCamera.hpp"

#include <glm/glm.hpp>

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
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform)
		{}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct CameraComponent {
		SceneCamera Camera;											// Scene Camera
		bool Active = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}