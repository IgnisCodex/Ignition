#include "IGPCH.hpp"
#include "Ignition/Scene/Object.hpp"

namespace Ignition::Scene{
	Object::Object(entt::entity handle, Scene* scene)
		: mObjectHandle(handle), mScene(scene)
	{}
}