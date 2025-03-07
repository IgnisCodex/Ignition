#include "IGPCH.hpp"
#include "Ignition/Graphics/Texture.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer.hpp"

#include "Backends/OpenGL/OpenGLTexture.hpp"

namespace Ignition::Graphics {
	Ref<Texture2D> Texture2D::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<Backends::OpenGLTexture2D>(filepath);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}
}