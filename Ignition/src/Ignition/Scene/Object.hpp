#pragma once

#include "Ignition/Scene/Scene.hpp"

#include <entt.hpp>

namespace Ignition::Scene {
	class Object {
	public:
		Object() = default;
		Object(entt::entity handle, Scene* scene);
		Object(const Object& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			IG_CORE_ASSERT(!HasComponent<T>(), "Object already has component!");
			return mScene->mRegistry.emplace<T>(mObjectHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			IG_CORE_ASSERT(HasComponent<T>(), "Object does not have component!");
			return mScene->mRegistry.get<T>(mObjectHandle);
		}

		template<typename T>
		bool HasComponent() {
			return mScene->mRegistry.all_of<T>(mObjectHandle);
		}

		template<typename T>
		void RemoveComponent() {
			IG_CORE_ASSERT(HasComponent<T>(), "Object does not have component!");
			mScene->mRegistry.remove<T>(mObjectHandle);
		}

		operator bool() const { return mObjectHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)mObjectHandle; }
		operator entt::entity() const { return mObjectHandle; }

		bool operator==(const Object& other) const { return mObjectHandle == other.mObjectHandle && mScene == other.mScene; }
		bool operator!=(const Object& other) const { return !(*this == other); }

	private:
		entt::entity mObjectHandle = entt::null;
		Scene* mScene = nullptr;
	};


	class ScriptableObject {
	public:
		virtual ~ScriptableObject() {}

		template<typename T>
		T& GetComponent() {
			return mObject.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Util::DeltaTime dt) {}

	private:
		Object mObject;
		friend class Scene;
	};
}