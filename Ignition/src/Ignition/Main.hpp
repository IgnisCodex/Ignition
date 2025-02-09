#pragma once

#ifdef IG_PLATFORM_WINDOWS
extern Ignition::Core::Application* Ignition::Core::CreateApplication();

int main(int argc, char** argv) {
	Ignition::Log::Init();
	auto app = Ignition::Core::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#endif