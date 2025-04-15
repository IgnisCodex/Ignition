#pragma once

#include "Ignition/Scene/Scene.hpp"

namespace Ignition::Scene {
	class Serialiser {
	public:
		Serialiser(const IGRef<Scene>& scene);

		void SerialiseYAML(const std::string& filepath);
		bool DeserialiseYAML(const std::string& filepath);

		void SerialiseRuntime(const std::string& filepath);
		bool DeserialiseRuntime(const std::string& filepath);
	private:
		IGRef<Scene> mScene;
	};
}