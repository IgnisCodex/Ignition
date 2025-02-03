#include <Ignition.hpp>

class Sandbox : public Ignition::Application {
public:
	Sandbox() {
		IG_INFO("Started!");
	}

	~Sandbox() {

	}
};

Ignition::Application* Ignition::CreateApplication() {
	return new Sandbox();
}