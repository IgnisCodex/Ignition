#include "IGPCH.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

#include "Ignition/Log.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace Ignition::Backends {
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		IG_CORE_ASSERT(false, "Unkown Shader Type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {
		std::string src = ReadFile(filepath);
		auto srcs = PreProcess(src);

		Compile(srcs);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
		: mRendererID(0)
	{
		std::unordered_map<GLenum, std::string> srcs;
		srcs[GL_VERTEX_SHADER] = vertexSrc;
		srcs[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(srcs);
	}

	// ---- Uniform Uploads ------------+
	void OpenGLShader::UploadInt(const std::string& name, int value) const {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1i(location, value);
	}

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

	// ---------- Private ----------

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& srcs) {
		GLuint program = glCreateProgram();

		std::vector<GLenum> glShaderIDs(srcs.size());

		for (auto& kv : srcs) {
			GLenum type = kv.first;
			const std::string& src = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* glSrc = src.c_str();
			glShaderSource(shader, 1, &glSrc, 0);
			glCompileShader(shader);

			GLint status = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> log(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]);

				glDeleteShader(shader);
				IG_CORE_CRITICAL("{}", log.data());
				IG_CORE_ASSERT(false, "Shader Compilation Failed!");
				return;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		mRendererID = program;

		glLinkProgram(mRendererID);

		GLint status = 0;
		glGetProgramiv(mRendererID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> log(maxLength);
			glGetProgramInfoLog(mRendererID, maxLength, &maxLength, &log[0]);

			glDeleteProgram(mRendererID);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			IG_CORE_CRITICAL("{}", log.data());
			IG_CORE_ASSERT(false, "Shader Linking Failed!");
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(mRendererID, id);

		mRendererID = program;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {
		std::string result;

		std::ifstream file(filepath, std::ios::in, std::ios::binary);
		if (file) {
			file.seekg(0, std::ios::end);
			result.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&result[0], result.size());
			file.close();

		} else {
			IG_CORE_ERROR("Could not Open File '{}'!", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& src) {
		std::unordered_map<GLenum, std::string> srcs;

		const char* token = "#type";
		size_t tokenLength = strlen(token);
		size_t pos = src.find(token);
		while (pos != std::string::npos) {
			size_t eol = src.find_first_of("\r\n", pos);
			IG_CORE_ASSERT(eol != std::string::npos, "Syntax Error!");
			size_t begin = pos + tokenLength + 1;
			std::string type = src.substr(begin, eol - begin);
			IG_CORE_ASSERT(ShaderTypeFromString(type), "Invalid Shader Type!");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(token, nextLinePos);
			srcs[ShaderTypeFromString(type)] = src.substr(
				nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos)
			);
		}

		return srcs;
	}

	
}