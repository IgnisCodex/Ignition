#include "IGPCH.hpp"
#include "Ignition/Graphics/RenderCall.hpp"

#include "Backends/OpenGL/OpenGLRendererAPI.hpp"

namespace Ignition::Graphics {
	RendererAPI* RenderCall::sRendererAPI = new Backends::OpenGLRendererAPI;
}