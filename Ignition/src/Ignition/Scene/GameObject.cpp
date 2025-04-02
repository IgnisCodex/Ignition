#include "IGPCH.hpp"
#include "Ignition/Scene/GameObject.hpp"

namespace Ignition::Scene{
	GameObject::GameObject(entt::entity handle, Scene* scene)
		: mGameObjectHandle(handle), mScene(scene)
	{}
}