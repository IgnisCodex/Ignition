#pragma once

namespace Ignition::Graphics {
	enum class API {
		None = 0,
		OpenGL = 1
	};

	class Renderer {
	public:
		inline static API GetAPI() { return sAPI; }
	private:
		static API sAPI;
	};
}
