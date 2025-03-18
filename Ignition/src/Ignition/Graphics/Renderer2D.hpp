#pragma once

#include "Ignition/Graphics/Camera.hpp"
#include "Ignition/Graphics/Texture.hpp"

namespace Ignition::Graphics {
	class Renderer2D {
	public:
		static void Init();
		static void Terminate();

		static void SceneBegin(const OrthoCamera& camera);
		static void SceneEnd();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const glm::vec4& colour);
		
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const IGRef<Texture2D>& texture);
		static void DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const IGRef<Texture2D>& texture);
	};
}