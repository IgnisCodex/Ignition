#include "IGPCH.hpp"
#include "Ignition/Graphics/Framebuffer.hpp"

#include "Ignition/Graphics/Renderer.hpp"
#include "Backends/OpenGL/OpenGLFramebuffer.hpp"

namespace Ignition::Graphics {
	IGRef<Framebuffer> Framebuffer::Create(const FramebufferProperties& properties) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLFramebuffer>(properties);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}
}