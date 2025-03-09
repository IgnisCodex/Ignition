#pragma once

#include "Ignition/Graphics/VertexArray.hpp"

#include <memory>
#include <glm/glm.hpp>

namespace Ignition::Graphics {
	class RendererAPI {
	public:
		enum class API {
			None =		0,
			OpenGL =	1
		};

		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void Clear(const glm::vec4& colour) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return sAPI; }
		
	private:
		static API sAPI;
	};
}