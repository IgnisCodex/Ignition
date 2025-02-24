#pragma once

#include "Ignition/Graphics/Buffers.hpp"

namespace Ignition::Backends {
	class OpenGLVertexBuffer : public Graphics::VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t mRendererID;
	};

	class OpenGLIndexBuffer : public Graphics::IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return mCount; }

	private:
		uint32_t mRendererID;
		uint32_t mCount;
	};
}