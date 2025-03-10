#include "IGPCH.hpp"
#include "Ignition/Graphics/Renderer.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer2D.hpp"

namespace Ignition::Graphics {
	void Renderer::Init() {
		RenderCall::Init();
		Renderer2D::Init();
	}

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
	// Events							|
	// =================================*
	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCall::SetViewport(0, 0, width, height);
	}

	// =================================*
	// Submit							|
	// =================================*
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->Bind();
		shader->UploadMatrix4f("u_ViewProjection", mScene->ViewProjectionMatrix);
		shader->UploadMatrix4f("u_Transform", transform);

		vertexArray->Bind();
		RenderCall::DrawIndexed(vertexArray);
	}
}