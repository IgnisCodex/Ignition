#pragma once

namespace Ignition::Graphics {
	class Context {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}