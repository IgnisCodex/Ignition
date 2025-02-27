#pragma once

#include "Ignition/Graphics/RendererAPI.hpp"

namespace Ignition::Graphics {
	class RenderCall {
	public:
		inline static void Clear(const glm::vec4 colour) {
			sRendererAPI->Clear(colour);
		}

		inline static void Clear() {
			sRendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			sRendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* sRendererAPI;
	};
}