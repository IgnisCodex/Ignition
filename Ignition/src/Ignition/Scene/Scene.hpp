#pragma once

#include "Ignition/Util/Time.hpp"

#include <entt.hpp>

namespace Ignition::Scene {
	class Object;

	class Scene {
	public:
		Scene();
		~Scene();

		Object CreateObject(const std::string& name = "");
		void DeleteObject(Object obj);
		
		void OnUpdate(Util::DeltaTime dt);
		void OnViewportResize(uint32_t width, uint32_t height);

		entt::registry& GetRegistry() { return mRegistry; }

	private:
		uint32_t mViewportWidth = 0, mViewportHeight = 0;

		entt::registry mRegistry;

		friend class Object;
		friend class Serialiser;
	};
}