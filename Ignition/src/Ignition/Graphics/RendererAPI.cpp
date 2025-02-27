#include "IGPCH.hpp"

#include "Ignition/Graphics/RendererAPI.hpp"

namespace Ignition::Graphics {
	RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OpenGL;
}