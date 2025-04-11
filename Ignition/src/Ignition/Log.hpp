#pragma once

#include "IGPCH.hpp"
#include "API.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Ignition {
	class Log {
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

#if defined (IG_PLATFORM_WINDOWS)
	#define IG_BREAK __debugbreak();
#elif defined (IG_PLATFORM_MACOS)
	#define IG_BREAK __builtin_debugtrap();
#else
	#define IG_BREAK __builtin_trap();
#endif

// TODO: Add a check for release type, then cast 0 to a void for each
#define IG_CORE_TRACE(...)    ::Ignition::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define IG_CORE_INFO(...)     ::Ignition::Log::GetCoreLogger()->info(__VA_ARGS__)
#define IG_CORE_WARN(...)     ::Ignition::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define IG_CORE_ERROR(...)    ::Ignition::Log::GetCoreLogger()->error(__VA_ARGS__)
#define IG_CORE_CRITICAL(...) ::Ignition::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define IG_CORE_ASSERT(x, msg) \
    if ((x)) {} else { IG_CORE_CRITICAL("{}\n\n\t{}\n\n\tFile: {}\n\tLine: {}", #x, msg, __FILE__, __LINE__); IG_BREAK }

#define IG_TRACE(...)         ::Ignition::Log::GetClientLogger()->trace(__VA_ARGS__)
#define IG_INFO(...)          ::Ignition::Log::GetClientLogger()->info(__VA_ARGS__)
#define IG_WARN(...)          ::Ignition::Log::GetClientLogger()->warn(__VA_ARGS__)
#define IG_ERROR(...)         ::Ignition::Log::GetClientLogger()->error(__VA_ARGS__)
#define IG_CRITICAL(...)      ::Ignition::Log::GetClientLogger()->critical(__VA_ARGS__)

#define IG_ASSERT(x, msg) \
    if ((x)) {} else { IG_CRITICAL("{}\n\n\t{}\n\n\tFile: {}\n\tLine: {}", #x, msg, __FILE__, __LINE__); IG_BREAK }