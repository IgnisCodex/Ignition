#pragma once

#include "Ignition/Graphics/Camera.hpp"
#include "Ignition/Graphics/RendererAPI.hpp"
#include "Ignition/Graphics/RenderCall.hpp"
#include "Ignition/Graphics/Shader.hpp"

#include <glm/glm.hpp>

namespace Ignition::Graphics {
	class Renderer {
	public:
		static bool SceneBegin(OrthoCamera& camera);
		static void SceneEnd();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct Scene {
			glm::mat4 ViewProjectionMatrix;
			bool State = false;
		};

		static Scene* mScene;
	};
}
