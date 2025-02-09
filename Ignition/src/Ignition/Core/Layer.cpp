#include "IGPCH.hpp"

#include "Ignition/Core/Layer.hpp"

namespace Ignition::Core {
	Layer::Layer(const std::string& debugName)
		: mDebugName(debugName)
	{}

	Layer::~Layer() {}
}