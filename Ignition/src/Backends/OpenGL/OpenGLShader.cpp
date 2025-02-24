#include "IGPCH.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

#include "Ignition/Log.hpp"

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

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(mRendererID);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}
}