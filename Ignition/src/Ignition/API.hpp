#pragma once

#include <memory>

#ifdef IG_PLATFORM_WINDOWS
	#ifdef IG_DYNAMIC_LINK
		#ifdef IG_BUILD_DLL
			#define IGNITION_API __declspec(dllexport)

		#else
			#define IGNITION_API __declspec(dllimport)
		#endif

	#else
		#define IGNITION_API
	#endif

#else
	#error Ignition only supports Windows!
#endif

#define IG_BIND_EVENT(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Ignition {
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;
}