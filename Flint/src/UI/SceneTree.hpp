#pragma once

#include <Ignition.hpp>

namespace Ignition::UI {
	class SceneTree {
	public:
		SceneTree() = default;
		SceneTree(const IGRef<Scene::Scene>& context);

		void SetContext(const IGRef<Scene::Scene>& context);
		void OnImGuiRender();

	private:
		void DrawTree(Scene::GameObject go);
		void DrawComponents(Scene::GameObject go);

	private:
		IGRef<Scene::Scene> mSceneContext;
		Scene::GameObject mSelectedGameObjectContext;
	};
}