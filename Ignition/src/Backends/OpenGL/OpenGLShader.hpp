#pragma once

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Shader.hpp"

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Ignition::Backends {
	
	class OpenGLShader : public Graphics::Shader {
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		// ---- Uniform Uploads ------------+
		virtual void UploadInt(const std::string& name, int value) const override;
		virtual void UploadFloat(const std::string& name, float value) const override;
		virtual void UploadVector2f(const std::string& name, const glm::vec2& vec2f) const override;
		virtual void UploadVector3f(const std::string& name, const glm::vec3& vec3f) const override;
		virtual void UploadVector4f(const std::string& name, const glm::vec4& vec4f) const override;

		virtual void UploadMatrix3f(const std::string& name, const glm::mat3& mat3f) const override;
		virtual void UploadMatrix4f(const std::string& name, const glm::mat4& mat4f) const override;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& src);
		void Compile(const std::unordered_map<GLenum, std::string>& srcs);

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