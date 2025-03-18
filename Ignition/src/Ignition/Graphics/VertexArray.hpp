#pragma once

#include <memory>
#include "Ignition/Graphics/Buffers.hpp"

namespace Ignition::Graphics {
	class VertexArray {
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const IGRef<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const IGRef<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<IGRef<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const IGRef<IndexBuffer>& GetIndexBuffer() const = 0;

	public:
		static IGRef<VertexArray> Create();
	};
}