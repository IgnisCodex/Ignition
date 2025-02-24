#pragma once

#include "Ignition/Graphics/Shader.hpp"

namespace Ignition::Backends {
	class OpenGLShader : public Graphics::Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t mRendererID;
	};
}