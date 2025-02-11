#include <Ignition.hpp>

class ExampleLayer : public Ignition::Core::Layer {
public:
	ExampleLayer() {}

	void OnUpdate() override {
		
	}

	void OnEvent(Ignition::Events::Event& event) override {
		//IG_TRACE("{}", event.GetName());
	}
};

class Sandbox : public Ignition::Core::Application {
public:
	Sandbox() {
		IG_INFO("Started!");
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Ignition::Core::Application* Ignition::Core::CreateApplication() {
	return new Sandbox();
}