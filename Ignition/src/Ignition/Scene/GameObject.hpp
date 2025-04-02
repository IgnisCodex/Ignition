#pragma once

#include "Ignition/Scene/Scene.hpp"

#include <entt.hpp>

namespace Ignition::Scene {
	class GameObject {
	public:
		GameObject() = default;
		GameObject(entt::entity handle, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			IG_CORE_ASSERT(!HasComponent<T>(), "GameObject already has component!");
			return mScene->mRegistry.emplace<T>(mGameObjectHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			IG_CORE_ASSERT(HasComponent<T>(), "GameObject does not have component!");
			return mScene->mRegistry.get<T>(mGameObjectHandle);
		}

		template<typename T>
		bool HasComponent() {
			return mScene->mRegistry.all_of<T>(mGameObjectHandle);
		}

		template<typename T>
		void RemoveComponent() {
			IG_CORE_ASSERT(HasComponent<T>(), "GameObject does not have component!");
			mScene->mRegistry.remove<T>(mGameObjectHandle);
		}

		operator bool() const { return mGameObjectHandle != entt::null; }

	private:
		entt::entity mGameObjectHandle = entt::null;
		Scene* mScene = nullptr;
	};
}