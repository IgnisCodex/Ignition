#pragma once

#include "Ignition/Graphics/Framebuffer.hpp"

namespace Ignition::Backends {
	class OpenGLFramebuffer : public Graphics::Framebuffer {
	public:
		OpenGLFramebuffer(const Graphics::FramebufferProperties& properties);
		virtual ~OpenGLFramebuffer();
		
		virtual void Bind() override;
		virtual void Unbind() override;
		void Resize();

		virtual const Graphics::FramebufferProperties& GetProperties() const override { return mProperties; }
		virtual uint32_t GetColourAttachmentRendererID() const override { return mColourAttachment; }

	private:
		uint32_t mRendererID;
		uint32_t mColourAttachment, mDepthAttachment;
		Graphics::FramebufferProperties mProperties;
	};
}