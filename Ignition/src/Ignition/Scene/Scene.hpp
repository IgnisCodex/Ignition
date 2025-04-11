#pragma once

#include "Ignition/Util/Time.hpp"

#include <entt.hpp>

namespace Ignition::Scene {
	class GameObject;

	class Scene {
	public:
		Scene();
		~Scene();

		GameObject CreateGameObject(const std::string& name = "");
		void DeleteGameObject(GameObject go);
		
		void OnUpdate(Util::DeltaTime dt);
		void OnViewportResize(uint32_t width, uint32_t height);

		entt::registry& GetRegistry() { return mRegistry; }

	private:
		uint32_t mViewportWidth = 0, mViewportHeight = 0;

		entt::registry mRegistry;

		friend class GameObject;
	};
}