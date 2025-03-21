#pragma once

#include "Ignition/Graphics/Buffers.hpp"

namespace Ignition::Backends {
	class OpenGLVertexBuffer : public Graphics::VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const Graphics::BufferLayout& GetLayout() const { return mLayout; }
		virtual void SetLayout(const Graphics::BufferLayout& layout) { mLayout = layout; };

		virtual void SetData(const void* data, uint32_t size) override;

	private:
		uint32_t mRendererID;
		Graphics::BufferLayout mLayout;

	};

	class OpenGLIndexBuffer : public Graphics::IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const { return mCount; }

	private:
		uint32_t mRendererID;
		uint32_t mCount;
	};
}