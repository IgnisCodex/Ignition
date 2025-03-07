#include "IGPCH.hpp"
#include "Backends/OpenGL/OpenGLTexture.hpp"

#include "Ignition/Log.hpp"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace Ignition::Backends {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		: mFilepath(filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		IG_CORE_ASSERT(data, "Failed to Load Texture!");
		mWidth = width;
		mHeight = height;

		GLenum openGLFormat = 0, dataFormat = 0;
		if (channels == 3) {
			openGLFormat = GL_RGB8;
			dataFormat = GL_RGB;

		} else if (channels == 4) {
			openGLFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		IG_CORE_ASSERT(openGLFormat & dataFormat, "Texture Format is not Supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, openGLFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &mRendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(0, mRendererID);
	}
}