#include "IGPCH.hpp"

#include "Ignition/Graphics/Shader.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

namespace Ignition::Graphics {
	IGRef<Shader> Shader::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLShader>(filepath);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}

	IGRef<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const IGRef<Shader>& shader) {
		IG_CORE_ASSERT(!Exists(name), "Shader already exists!");
		mShaders[name] = shader;
	}

	void ShaderLibrary::Add(const IGRef<Shader>& shader) {
		auto& name = shader->GetName();
		Add(name, shader);
	}

	IGRef<Shader> ShaderLibrary::Load(const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	IGRef<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	IGRef<Shader> ShaderLibrary::Get(const std::string& name) {
		IG_CORE_ASSERT(Exists(name), "Shader not found!");
		return mShaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const {
		return mShaders.find(name) != mShaders.end();
	}
}