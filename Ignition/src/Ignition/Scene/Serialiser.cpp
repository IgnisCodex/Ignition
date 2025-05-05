#include "IGPCH.hpp"
#include "Ignition/Scene/Serialiser.hpp"

#include "Ignition/Scene/SceneCamera.hpp"
#include "Ignition/Scene/Components.hpp"
#include "Ignition/Scene/Object.hpp"

#include <yaml-cpp/yaml.h>

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Ignition::Scene {
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	
	Serialiser::Serialiser(const IGRef<Scene>& scene)
		: mScene(scene)
	{}

	static void SerialiseObject(YAML::Emitter& out, Object obj) {
		out << YAML::BeginMap;
		out << YAML::Key << "Object" << YAML::Value << "123456789"; // UUID;

		if (obj.HasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent" << YAML::BeginMap;

			auto& tag = obj.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (obj.HasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent" << YAML::BeginMap;

			auto& tc = obj.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}

		if (obj.HasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent" << YAML::BeginMap;

			auto& cc = obj.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;

			out << YAML::Key << "Camera" << YAML::BeginMap;
			
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspVFOV" << YAML::Value << camera.GetPerspVFOV();
			out << YAML::Key << "PerspNear" << YAML::Value << camera.GetPerspNearClip();
			out << YAML::Key << "PerspFar" << YAML::Value << camera.GetPerspFarClip();
			out << YAML::Key << "OrthoSize" << YAML::Value << camera.GetOrthoSize();
			out << YAML::Key << "OrthoNear" << YAML::Value << camera.GetOrthoNearClip();
			out << YAML::Key << "OrthoFar" << YAML::Value << camera.GetOrthoFarClip();
			
			out << YAML::EndMap;
			
			out << YAML::Key << "Active" << YAML::Value << cc.Active;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (obj.HasComponent<SpriteRendererComponent>()) {
			out << YAML::Key << "SpriteRendererComponent" << YAML::BeginMap;

			auto& src = obj.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Colour" << YAML::Value << src.Colour;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void Serialiser::SerialiseYAML(const std::string& filepath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
		auto& reg = mScene->GetRegistry();

		auto view = reg.view<entt::entity>();
		view.each([&](auto objID) {
			Object obj = { objID, mScene.get() };
			if (!obj) return;
			SerialiseObject(out, obj);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool Serialiser::DeserialiseYAML(const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strstr;
		strstr << stream.rdbuf();

		YAML::Node data = YAML::Load(strstr.str());
		if (!data["Scene"])
			return false;

		std::string name = data["Scene"].as<std::string>();
		IG_CORE_TRACE("Loading scene '{}'...", name);
		
		auto objs = data["Objects"];
		if (objs) {
			for (auto obj : objs) {
				uint64_t uuid = obj["Object"].as<uint64_t>();
				std::string tag;
				auto tc = obj["TagComponent"];
				if (tc)
					tag = tc["Tag"].as<std::string>();

				IG_CORE_TRACE("Object {}: '{}' loaded.", uuid, tag);

				Object deserialisedobj = mScene->CreateObject(tag);

				auto transformC = obj["TransformComponent"];
				if (transformC) {
					// Objects always have a Transform Component on creation.
					auto& tc = deserialisedobj.GetComponent<TransformComponent>();
					tc.Translation = transformC["Translation"].as<glm::vec3>();
					tc.Rotation = transformC["Rotation"].as<glm::vec3>();
					tc.Scale = transformC["Scale"].as<glm::vec3>();
				}

				auto cameraC = obj["CameraComponent"];
				if (cameraC) {
					auto& cc = deserialisedobj.AddComponent<CameraComponent>();

					auto& camera = cameraC["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());
					
					cc.Camera.SetPerspVFOV(camera["PerspVFOV"].as<float>());
					cc.Camera.SetPerspNearClip(camera["PerspNear"].as<float>());
					cc.Camera.SetPerspFarClip(camera["PerspFar"].as<float>());

					cc.Camera.SetOrthoSize(camera["OrthoSize"].as<float>());
					cc.Camera.SetOrthoNearClip(camera["OrthoNear"].as<float>());
					cc.Camera.SetOrthoFarClip(camera["OrthoFar"].as<float>());

					cc.Active = cameraC["Active"].as<bool>();
					cc.FixedAspectRatio = cameraC["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererC = obj["SpriteRendererComponent"];
				if (spriteRendererC) {
					auto& src = deserialisedobj.AddComponent<SpriteRendererComponent>();
					src.Colour = spriteRendererC["Colour"].as<glm::vec4>();
				}
			}
		}

		IG_CORE_TRACE("Scene '{}' loaded successfully.", name);

		return true;
	}

	void Serialiser::SerialiseRuntime(const std::string& filepath) {
		IG_CORE_ASSERT(false, "Not Implamented!");
	}

	bool Serialiser::DeserialiseRuntime(const std::string& filepath) {
		IG_CORE_ASSERT(false, "Not Implamented!");
		return false;
	}
}