#pragma once

#include "Ignition/API.hpp"
#include "Ignition/Graphics/Texture.hpp"

#include <string>

namespace Ignition::Graphics {
	class Texture {
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static IGRef<Texture2D> Create(uint32_t width, uint32_t height);
		static IGRef<Texture2D> Create(const std::string& filepath);
	};



	class SubTexture2D {
	public:
		SubTexture2D(const IGRef<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		
		const IGRef<Texture2D> GetTexture() { return mTexture; }
		const glm::vec2* GetTextureCoordinates() { return mTextureCoordinates; }

	public:
		//static IGRef<SubTexture2D> Create(const IGRef<Texture2D>& texture, int index, int size);
		static IGRef<SubTexture2D> Create(const IGRef<Texture2D>& texture, glm::vec2& coordinates, glm::vec2& size);
		
	private:
		IGRef<Texture2D> mTexture;
		glm::vec2 mTextureCoordinates[4];
	};
}