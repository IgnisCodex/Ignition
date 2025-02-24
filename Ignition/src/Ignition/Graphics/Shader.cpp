#include "IGPCH.hpp"

#include "Ignition/Graphics/Shader.hpp"

#include "Ignition/Log.hpp"

#include <glad/glad.h>

namespace Ignition::Graphics {
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
		: mID(0)
	{
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

		mID = glCreateProgram();

		glAttachShader(mID, vertexShader);
		glAttachShader(mID, fragmentShader);

		glLinkProgram(mID);

		status = 0;
		glGetProgramiv(mID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> log(maxLength);
			glGetProgramInfoLog(mID, maxLength, &maxLength, &log[0]);

			glDeleteProgram(mID);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			IG_CORE_CRITICAL("{}", log.data());
			IG_CORE_ASSERT(false, "Shader Linking Failed!");
			return;
		}

		glDetachShader(mID, vertexShader);
		glDetachShader(mID, fragmentShader);
	}

	Shader::~Shader() {
		glDeleteProgram(mID);
	}

	void Shader::Bind() const {
		glUseProgram(mID);
	}

	void Shader::Unbind() const {
		glUseProgram(0);
	}
}