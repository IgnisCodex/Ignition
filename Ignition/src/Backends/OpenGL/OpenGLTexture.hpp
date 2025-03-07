#pragma once

#include "Ignition/Graphics/Texture.hpp"

namespace Ignition::Backends {
	class OpenGLTexture2D : public Graphics::Texture2D {
	public:
		OpenGLTexture2D(const std::string& filepath);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) const override;

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }

	private:
		std::string mFilepath;
		uint32_t mWidth;
		uint32_t mHeight;

		uint32_t mRendererID;
	};
}