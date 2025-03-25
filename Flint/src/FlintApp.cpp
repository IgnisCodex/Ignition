#include <Ignition.hpp>
#include <Ignition/Main.hpp>

#include "EditorLayer.hpp"

namespace Ignition {
	class Flint : public Core::Application {
	public:
		Flint()
			: Application("Flint")
		{
			PushLayer(new EditorLayer());
		}

		~Flint() {}
	};

	Core::Application* Core::CreateApplication() {
		return new Flint();
	}
}