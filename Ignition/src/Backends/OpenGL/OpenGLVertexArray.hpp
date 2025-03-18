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

		virtual void AddVertexBuffer(const IGRef<Graphics::VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const IGRef<Graphics::IndexBuffer>& indexBuffer) override;

		virtual const std::vector<IGRef<Graphics::VertexBuffer>>& GetVertexBuffers() const { return mVertexBuffers; };
		virtual const IGRef<Graphics::IndexBuffer>& GetIndexBuffer() const { return mIndexBuffer; };

	private:
		uint32_t mRendererID;

		std::vector<IGRef<Graphics::VertexBuffer>> mVertexBuffers;
		IGRef<Graphics::IndexBuffer> mIndexBuffer;
	};
}