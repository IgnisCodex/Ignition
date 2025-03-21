#pragma once

#include "Ignition/Graphics/RendererAPI.hpp"

namespace Ignition::Graphics {
	class RenderCall {
	public:
		inline static void Init() {
			sRendererAPI->Init();
		}

		inline static void Clear(const glm::vec4 colour) {
			sRendererAPI->Clear(colour);
		}

		inline static void Clear() {
			sRendererAPI->Clear();
		}

		inline static void DrawIndexed(const IGRef<VertexArray>& vertexArray, uint32_t count = 0) {
			sRendererAPI->DrawIndexed(vertexArray, count);
		}


		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			sRendererAPI->SetViewport(x, y, width, height);
		}

	private:
		static RendererAPI* sRendererAPI;
	};
}