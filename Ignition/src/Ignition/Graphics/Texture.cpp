#include "IGPCH.hpp"
#include "Ignition/Graphics/Texture.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer.hpp"

#include "Backends/OpenGL/OpenGLTexture.hpp"

namespace Ignition::Graphics {
	IGRef<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLTexture2D>(width, height);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}

	IGRef<Texture2D> Texture2D::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLTexture2D>(filepath);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}
}