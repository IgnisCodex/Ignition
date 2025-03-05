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

		virtual void AddVertexBuffer(const Ref<Graphics::VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<Graphics::IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<Graphics::VertexBuffer>>& GetVertexBuffers() const { return mVertexBuffers; };
		virtual const Ref<Graphics::IndexBuffer>& GetIndexBuffer() const { return mIndexBuffer; };

	private:
		uint32_t mRendererID;

		std::vector<Ref<Graphics::VertexBuffer>> mVertexBuffers;
		Ref<Graphics::IndexBuffer> mIndexBuffer;
	};
}