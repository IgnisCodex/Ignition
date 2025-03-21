#include "IGPCH.hpp"

#include "Ignition/Graphics/Buffers.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer.hpp"
#include "Ignition/Graphics/RendererAPI.hpp"

#include "Backends/OpenGL/OpenGLBuffers.hpp"

namespace Ignition::Graphics {
	IGRef<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLVertexBuffer>(size);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}

	IGRef<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLVertexBuffer>(vertices, size);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}


	IGRef<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLIndexBuffer>(indices, count);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}
}