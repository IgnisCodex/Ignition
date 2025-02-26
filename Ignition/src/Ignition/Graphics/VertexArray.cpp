#include "IGPCH.hpp"

#include "Ignition/Graphics/VertexArray.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer.hpp"

#include "Backends/OpenGL/OpenGLVertexArray.hpp"

namespace Ignition::Graphics {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case API::OpenGL:
			return new Backends::OpenGLVertexArray();
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}
}