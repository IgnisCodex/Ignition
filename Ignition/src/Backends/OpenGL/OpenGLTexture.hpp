#pragma once

#include "Ignition/Graphics/Texture.hpp"

#include <glad/glad.h>

namespace Ignition::Backends {
	class OpenGLTexture2D : public Graphics::Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filepath);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void SetData(void* data, uint32_t size) override;

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }

		virtual bool operator==(const Texture& other) const override { return mRendererID == ((OpenGLTexture2D&)other).mRendererID; }

	private:
		std::string mFilepath;
		uint32_t mWidth;
		uint32_t mHeight;
		uint32_t mRendererID;
		GLenum mOpenGLFormat, mDataFormat;
	};
}