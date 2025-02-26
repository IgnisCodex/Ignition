#pragma once

#include "Ignition/Graphics/VertexArray.hpp"

#include "Ignition/Graphics/Buffers.hpp"

namespace Ignition::Backends {
	class OpenGLVertexArray : public Graphics::VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<Graphics::VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<Graphics::IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<Graphics::VertexBuffer>>& GetVertexBuffers() const { return mVertexBuffers; };
		virtual const std::shared_ptr<Graphics::IndexBuffer>& GetIndexBuffer() const { return mIndexBuffer; };

	private:
		uint32_t mRendererID;

		std::vector<std::shared_ptr<Graphics::VertexBuffer>> mVertexBuffers;
		std::shared_ptr<Graphics::IndexBuffer> mIndexBuffer;
	};
}