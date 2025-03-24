#pragma once

#include "Ignition/API.hpp"

namespace Ignition::Graphics {
	struct FramebufferProperties {
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const FramebufferProperties& GetProperties() const = 0;
		virtual uint32_t GetColourAttachmentRendererID() const = 0;

	public:
		static IGRef<Framebuffer> Create(const FramebufferProperties& properties);
	};
}