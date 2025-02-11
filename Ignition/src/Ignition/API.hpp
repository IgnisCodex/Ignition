#pragma once

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

#define BIT(x) (1 << (x))