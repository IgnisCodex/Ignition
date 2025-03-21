#pragma once

#ifdef IG_PLATFORM_WINDOWS
extern Ignition::Core::Application* Ignition::Core::CreateApplication();

int main(int argc, char** argv) {
	IG_PROFILING_BEGIN_SESSION("Init", "IGProfile-Init.json");
	Ignition::Log::Init();
	IG_PROFILING_END_SESSION();

	IG_PROFILING_BEGIN_SESSION("Runtime", "IGProfile-Runtime.json");
	auto app = Ignition::Core::CreateApplication();
	app->Run();
	IG_PROFILING_END_SESSION();

	IG_PROFILING_BEGIN_SESSION("Shutdown", "IGProfile-Shutdown.json");
	delete app;
	IG_PROFILING_END_SESSION();

	return 0;
}
#endif