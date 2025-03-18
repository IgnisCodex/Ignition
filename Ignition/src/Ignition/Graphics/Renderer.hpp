#pragma once

#include "Ignition/Graphics/Camera.hpp"
#include "Ignition/Graphics/RendererAPI.hpp"
#include "Ignition/Graphics/RenderCall.hpp"
#include "Ignition/Graphics/Shader.hpp"

#include <glm/glm.hpp>

namespace Ignition::Graphics {
	class Renderer {
	public:
		static void Init();
		
		static bool SceneBegin(OrthoCamera& camera);
		static void SceneEnd();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Submit(const IGRef<Shader>& shader, const IGRef<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct Scene {
			glm::mat4 ViewProjectionMatrix;
			bool State = false;
		};

		static Scene* mScene;
	};
}
