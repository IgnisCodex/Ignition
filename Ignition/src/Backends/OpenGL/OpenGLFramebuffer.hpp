#pragma once

#include "Ignition/Graphics/Framebuffer.hpp"

namespace Ignition::Backends {
	class OpenGLFramebuffer : public Graphics::Framebuffer {
	public:
		OpenGLFramebuffer(const Graphics::FramebufferProperties& properties);
		virtual ~OpenGLFramebuffer();
		
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		
		void Invalidate();

		virtual const Graphics::FramebufferProperties& GetProperties() const override { return mProperties; }
		virtual uint32_t GetColourAttachmentRendererID() const override { return mColourAttachment; }

	private:
		uint32_t mRendererID = 0;
		uint32_t mColourAttachment = 0, mDepthAttachment = 0;
		Graphics::FramebufferProperties mProperties;
	};
}