#pragma once

#include "Ignition/API.hpp"

namespace Ignition::Graphics {
	struct FramebufferProperties {
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const FramebufferProperties& GetProperties() const = 0;
		virtual uint32_t GetColourAttachmentRendererID() const = 0;

	public:
		static IGRef<Framebuffer> Create(const FramebufferProperties& properties);
	};
}