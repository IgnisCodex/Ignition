#pragma once

#include "Ignition/Graphics/RendererAPI.hpp"
#include "Ignition/Graphics/RenderCall.hpp"

namespace Ignition::Graphics {
	class Renderer {
	public:
		static bool SceneBegin();
		static void SceneEnd();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
