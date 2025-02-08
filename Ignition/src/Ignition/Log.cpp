// Header File
#include "IGPCH.hpp"
#include "Log.hpp"

// External Libraries
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Ignition {

	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		sCoreLogger = spdlog::stdout_color_mt("IGNITION");
		sCoreLogger->set_level(spdlog::level::trace);

		sClientLogger = spdlog::stdout_color_mt("APP");
		sClientLogger->set_level(spdlog::level::trace);
	}
}