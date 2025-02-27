#include "IGPCH.hpp"

#include "Ignition/Graphics/Renderer.hpp"

namespace Ignition::Graphics {
	// =================================*
	// Scenes							|
	// =================================*
	bool Renderer::SceneBegin() {
		return true;
	}

	void Renderer::SceneEnd() {

	}

	// =================================*
	// Submit							|
	// =================================*
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
		vertexArray->Bind();
		RenderCall::DrawIndexed(vertexArray);
	}
}