#include "IGPCH.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

#include "Ignition/Log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Ignition::Backends {
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
		: mRendererID(0) {
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* src = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &src, 0);
		glCompileShader(vertexShader);

		GLint status = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> log(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &log[0]);

			glDeleteShader(vertexShader);
			IG_CORE_CRITICAL("{}", log.data());
			IG_CORE_ASSERT(false, "Vertex Shader Compilation Failed!");
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		src = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &src, 0);
		glCompileShader(fragmentShader);

		status = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> log(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &log[0]);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			IG_CORE_CRITICAL("{}", log.data());
			IG_CORE_ASSERT(false, "Fragment Shader Compilation Failed!");
			return;
		}

		mRendererID = glCreateProgram();

		glAttachShader(mRendererID, vertexShader);
		glAttachShader(mRendererID, fragmentShader);

		glLinkProgram(mRendererID);

		status = 0;
		glGetProgramiv(mRendererID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> log(maxLength);
			glGetProgramInfoLog(mRendererID, maxLength, &maxLength, &log[0]);

			glDeleteProgram(mRendererID);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			IG_CORE_CRITICAL("{}", log.data());
			IG_CORE_ASSERT(false, "Shader Linking Failed!");
			return;
		}

		glDetachShader(mRendererID, vertexShader);
		glDetachShader(mRendererID, fragmentShader);
	}

	// ---- Uniform Uploads ------------+
	void OpenGLShader::UploadFloat(const std::string& name, float value) const {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadVector2f(const std::string& name, const glm::vec2& vec2f) const {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform2f(location, vec2f.x, vec2f.y);
	}

	void OpenGLShader::UploadVector3f(const std::string& name, const glm::vec3& vec3f) const {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform3f(location, vec3f.x, vec3f.y, vec3f.z);
	}

	void OpenGLShader::UploadVector4f(const std::string& name, const glm::vec4& vec4f) const {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform4f(location, vec4f.x, vec4f.y, vec4f.z, vec4f.w);
	}

	void OpenGLShader::UploadMatrix3f(const std::string& name, const glm::mat3& mat3f) const {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat3f));
	}

	void OpenGLShader::UploadMatrix4f(const std::string& name, const glm::mat4& mat4f) const  {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4f));
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(mRendererID);
	}

	void OpenGLShader::OpenGLShader::Bind() const {
		glUseProgram(mRendererID);
	}

	void OpenGLShader::OpenGLShader::Unbind() const {
		glUseProgram(0);
	}
}