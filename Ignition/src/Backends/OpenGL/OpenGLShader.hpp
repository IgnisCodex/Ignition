#pragma once

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Shader.hpp"

#include <glad/glad.h>

namespace Ignition::Backends {
	
	class OpenGLShader : public Graphics::Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t mRendererID;
	};


	static GLenum DataTypetoOpenGLBaseType(Graphics::DataType type) {
		switch (type) {
		case Graphics::DataType::Bool:
			return GL_BOOL;

		case Graphics::DataType::Float:
		case Graphics::DataType::Vector2f:
		case Graphics::DataType::Vector3f:
		case Graphics::DataType::Vector4f:
			return GL_FLOAT;

		case Graphics::DataType::Int:
		case Graphics::DataType::Vector2i:
		case Graphics::DataType::Vector3i:
		case Graphics::DataType::Vector4i:
			return GL_INT;

		case Graphics::DataType::Matrix3f:
		case Graphics::DataType::Matrix4f:
			return GL_FLOAT;
		}

		IG_CORE_ASSERT(false, "Invalid Data Type!");
		return 0;
	}
}