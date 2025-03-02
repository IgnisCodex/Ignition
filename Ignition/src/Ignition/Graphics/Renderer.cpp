#include "IGPCH.hpp"
#include "Ignition/Graphics/Renderer.hpp"

#include "Ignition/Log.hpp"

namespace Ignition::Graphics {
	// =================================*
	// Scenes							|
	// =================================*
	Renderer::Scene* Renderer::mScene = new Renderer::Scene;

	bool Renderer::SceneBegin(OrthoCamera& camera) {
		mScene->State = true;

		mScene->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		
		return true;
	}

	void Renderer::SceneEnd() {
		IG_CORE_ASSERT(mScene->State, "Tried to call SceneEnd() before SceneBegin()!");
		mScene->State = false;
	}

	// =================================*
	// Submit							|
	// =================================*
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->Bind();
		shader->UploadMatrix4f("u_ViewProjection", mScene->ViewProjectionMatrix);
		shader->UploadMatrix4f("u_Transform", transform);

		vertexArray->Bind();
		RenderCall::DrawIndexed(vertexArray);
	}
}