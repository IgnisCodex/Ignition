#include "IGPCH.hpp"
#include "Ignition/Util/Colour.hpp"

#include "Ignition/Log.hpp"

namespace Ignition::Util {
	glm::vec4 rgb(glm::vec3 colour) {
		IG_CORE_ASSERT(
			(colour.r >= 0 && colour.r <= 255) &&
			(colour.g >= 0 && colour.g <= 255) &&
			(colour.b >= 0 && colour.b <= 255),
			"Invalid Colour! Red, Green, and Blue must be 0 to 255."
		);

		return glm::vec4(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, 1.0f);

	}

	glm::vec4 rgb(float r, float g, float b) {
		return rgb({ r, g, b });
	}


	glm::vec4 rgba(glm::vec4 colour) {
		IG_CORE_ASSERT(
			(colour.r >= 0 && colour.r <= 255) &&
			(colour.g >= 0 && colour.g <= 255) &&
			(colour.b >= 0 && colour.b <= 255) &&
			(colour.a >= 0 && colour.a <= 1),
			"Invalid Colour! Red, Green, and Blue must be 0 to 255. Alpha must be 0 to 1."
		);

		return glm::vec4(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, colour.a);
	}

	glm::vec4 rgba(float r, float g, float b, float a) {
		return rgba({ r, g, b, a });
	}
}