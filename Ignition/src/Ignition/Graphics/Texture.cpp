#include "IGPCH.hpp"
#include "Ignition/Graphics/Texture.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Graphics/Renderer.hpp"

#include "Backends/OpenGL/OpenGLTexture.hpp"

namespace Ignition::Graphics {
	IGRef<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLTexture2D>(width, height);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}

	IGRef<Texture2D> Texture2D::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			IG_CORE_ASSERT(false, "Headless Mode is Currently not Supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return IGCreateRef<Backends::OpenGLTexture2D>(filepath);
		}

		IG_CORE_ASSERT(false, "Unknown Graphics API!");
		return nullptr;
	}


	SubTexture2D::SubTexture2D(const IGRef<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: mTexture(texture)
	{
		mTextureCoordinates[0] = { min.x, min.y };
		mTextureCoordinates[1] = { max.x, min.y };
		mTextureCoordinates[2] = { max.x, max.y };
		mTextureCoordinates[3] = { min.x, max.y };
	}

	/*

		64 * 64

		4 textures per length

		16 * 16

		size of tex width / size = how many textures per width
		size of tex height / size = how many textures per height

		index =

		*/

	/*IGRef<SubTexture2D> SubTexture2D::Create(const IGRef<Texture2D>& texture, int index, int size) {
		
	}*/

	IGRef<SubTexture2D> SubTexture2D::Create(const IGRef<Texture2D>& texture, glm::vec2& coordinates, glm::vec2& size) {
		
		glm::vec2 min = { (coordinates.x * size.x) / texture->GetWidth(), (coordinates.y * size.y) / texture->GetHeight() };
		glm::vec2 max = { ((coordinates.x + 1) * size.x) / texture->GetWidth(), ((coordinates.y + 1) * size.y) / texture->GetHeight() };
		
		return IGCreateRef<SubTexture2D>(texture, min, max);
	}
}