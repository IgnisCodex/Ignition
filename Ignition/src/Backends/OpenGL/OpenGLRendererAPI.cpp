#include "IGPCH.hpp"
#include "Backends/OpenGL/OpenGLRendererAPI.hpp"

#include "Ignition/Util/Util.hpp"

#include <glad/glad.h>

namespace Ignition::Backends {
	
	void OpenGLRendererAPI::Clear(const glm::vec4& colour) {
		glClearColor(colour.r, colour.g, colour.b, colour.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<Graphics::VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}