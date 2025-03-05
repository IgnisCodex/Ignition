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

		virtual void Clear() = 0;
		virtual void Clear(const glm::vec4& colour) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return sAPI; }
		
	private:
		static API sAPI;
	};
}