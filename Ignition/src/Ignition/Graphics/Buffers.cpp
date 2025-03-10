#include "IGPCH.hpp"

#include "Ignition/Graphics/Buffers.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer.hpp"
#include "Ignition/Graphics/RendererAPI.hpp"

#include "Backends/OpenGL/OpenGLBuffers.hpp"

namespace Ignition::Graphics {
	
	
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<Backends::OpenGLVertexBuffer>(vertices, size);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}


	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<Backends::OpenGLIndexBuffer>(indices, count);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}
}