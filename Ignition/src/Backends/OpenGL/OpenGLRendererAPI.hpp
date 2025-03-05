#pragma once

#include "Ignition/Graphics/RendererAPI.hpp"

namespace Ignition::Backends {
	class OpenGLRendererAPI : public Graphics::RendererAPI {
	public:
		virtual void Clear() override;
		virtual void Clear(const glm::vec4& colour) override;

		virtual void DrawIndexed(const Ref<Graphics::VertexArray>& vertexArray) override;
	};
}