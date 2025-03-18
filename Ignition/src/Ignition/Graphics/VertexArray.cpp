#include "IGPCH.hpp"

#include "Ignition/Graphics/VertexArray.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer.hpp"
#include "Ignition/Graphics/RendererAPI.hpp"

#include "Backends/OpenGL/OpenGLVertexArray.hpp"

namespace Ignition::Graphics {
	IGRef<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<Backends::OpenGLVertexArray>();
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}
}