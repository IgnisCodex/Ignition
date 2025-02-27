#pragma once

#include <glm/glm.hpp>

namespace Ignition::Util {
	glm::vec4 rgb(glm::vec3 colour);
	glm::vec4 rgb(float r, float g, float b);

	glm::vec4 rgba(glm::vec4 colour);
	glm::vec4 rgba(float r, float g, float b, float a);
}