#pragma once

#include "Ignition/Graphics/RendererAPI.hpp"

namespace Ignition::Backends {
	class OpenGLRendererAPI : public Graphics::RendererAPI {
	public:
		virtual void Init() override;
		virtual void Clear() override;
		virtual void Clear(const glm::vec4& colour) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const IGRef<Graphics::VertexArray>& vertexArray) override;
	};
}