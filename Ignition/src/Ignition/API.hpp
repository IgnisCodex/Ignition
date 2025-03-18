#pragma once

#include "Ignition/Graphics/Renderer2D.hpp"
#include "Ignition/Util/Colour.hpp"

#include <memory>

#include <glm/glm.hpp>

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

// ---- Igniton Typedefs ----
template <typename T>
using IGScope = std::unique_ptr<T>;

template <typename T>
using IGRef = std::shared_ptr<T>;

//using IGRenderer2D = Ignition::Graphics::Renderer2D;

// ---------------
//  Util | Colour
// ---------------
using IGColour = glm::vec4;
using Ignition::Util::rgb;
using Ignition::Util::rgba;