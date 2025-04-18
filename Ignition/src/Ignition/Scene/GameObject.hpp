#pragma once

#include "Ignition/Scene/Scene.hpp"

#include <entt.hpp>

namespace Ignition::Scene {
	class GameObject {
	public:
		GameObject() = default;
		GameObject(entt::entity handle, Scene* scene);
		GameObject(const GameObject& other) = default;

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
		operator uint32_t() const { return (uint32_t)mGameObjectHandle; }
		operator entt::entity() const { return mGameObjectHandle; }

		bool operator==(const GameObject& other) const { return mGameObjectHandle == other.mGameObjectHandle && mScene == other.mScene; }
		bool operator!=(const GameObject& other) const { return !(*this == other); }

	private:
		entt::entity mGameObjectHandle = entt::null;
		Scene* mScene = nullptr;
	};


	class ScriptableGameObject {
	public:
		virtual ~ScriptableGameObject() {}

		template<typename T>
		T& GetComponent() {
			return mGameObject.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Util::DeltaTime dt) {}

	private:
		GameObject mGameObject;
		friend class Scene;
	};
}