#include <Ignition.hpp>

class ExampleLayer : public Ignition::Core::Layer {
public:
	ExampleLayer() {}

	void OnUpdate() override {
		IG_INFO("ExampleLayer::OnUpdate()");
	}

	void OnEvent(Ignition::Events::Event& event) override {
		IG_TRACE("{}", event.GetName());
	}
};

class Sandbox : public Ignition::Application {
public:
	Sandbox() {
		IG_INFO("Started!");
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Ignition::Application* Ignition::CreateApplication() {
	return new Sandbox();
}