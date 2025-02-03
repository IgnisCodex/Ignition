#pragma once

#ifdef IG_PLATFORM_WINDOWS
extern Ignition::Application* Ignition::CreateApplication();

int main(int argc, char** argv) {
	Ignition::Log::Init();
	auto app = Ignition::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#endif