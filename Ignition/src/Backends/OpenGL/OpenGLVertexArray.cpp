#include "IGPCH.hpp"

#include "Backends/OpenGL/OpenGLVertexArray.hpp"

#include "Ignition/Log.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

#include <glad/glad.h>

namespace Ignition::Backends {
	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &mRendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &mRendererID);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(mRendererID);
	}
	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<Graphics::VertexBuffer>& vertexBuffer) {
		IG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer Layout is Invalid!");

		glBindVertexArray(mRendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				DataTypetoOpenGLBaseType(element.Type),
				element.Normalised ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		mVertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<Graphics::IndexBuffer>& indexBuffer) {
		glBindVertexArray(mRendererID);
		indexBuffer->Bind();

		mIndexBuffer = indexBuffer;
	}
}