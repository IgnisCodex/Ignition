#include "IGPCH.hpp"
#include "Backends/OpenGL/OpenGLFramebuffer.hpp"

#include <glad/glad.h>

namespace Ignition::Backends {
	OpenGLFramebuffer::OpenGLFramebuffer(const Graphics::FramebufferProperties& properties)
		: mProperties(properties)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(1, &mColourAttachment);
		glDeleteTextures(1, &mDepthAttachment);
	}

	void OpenGLFramebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glViewport(0, 0, mProperties.Width, mProperties.Height);
	}

	void OpenGLFramebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
		mProperties.Width = width;
		mProperties.Height = height;
		Invalidate();
	}
	
	void OpenGLFramebuffer::Invalidate() {
		if (mRendererID) {
			glDeleteFramebuffers(1, &mRendererID);
			glDeleteTextures(1, &mColourAttachment);
			glDeleteTextures(1, &mDepthAttachment);
		}

		glCreateFramebuffers(1, &mRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &mColourAttachment);
		glBindTexture(GL_TEXTURE_2D, mColourAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mProperties.Width, mProperties.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColourAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
		glBindTexture(GL_TEXTURE_2D, mDepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mProperties.Width, mProperties.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

		IG_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}