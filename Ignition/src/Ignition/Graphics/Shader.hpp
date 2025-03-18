#pragma once

#include "Ignition/Log.hpp"

#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

namespace Ignition::Graphics {

	enum class DataType {
		Bool,

		Float,
		Vector2f,
		Vector3f,
		Vector4f,

		Int,
		Vector2i,
		Vector3i,
		Vector4i,

		Matrix3f,
		Matrix4f
	};

	static uint32_t GetDataTypeSize(DataType type) {
		switch (type) {

		case DataType::Bool:
			return 1;

		case DataType::Float:
			return 4;
		case DataType::Vector2f:
			return 4 * 2;
		case DataType::Vector3f:
			return 4 * 3;
		case DataType::Vector4f:
			return 4 * 4;

		case DataType::Int:
			return 4;
		case DataType::Vector2i:
			return 4 * 2;
		case DataType::Vector3i:
			return 4 * 3;
		case DataType::Vector4i:
			return 4 * 4;

		case DataType::Matrix3f:
			return 4 * 3 * 3;
		case DataType::Matrix4f:
			return 4 * 4 * 4;
		}

		IG_CORE_ASSERT(false, "Invalid Data Type!");
		return 0;
	}

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		// ---- Uniform Uploads ------------+
		virtual void UploadInt(const std::string& name, int value) const = 0;
		virtual void UploadFloat(const std::string& name, float value) const = 0;
		virtual void UploadVector2f(const std::string& name, const glm::vec2& vec2f) const = 0;
		virtual void UploadVector3f(const std::string& name, const glm::vec3& vec3f) const = 0;
		virtual void UploadVector4f(const std::string& name, const glm::vec4& vec4f) const = 0;

		virtual void UploadMatrix3f(const std::string& name, const glm::mat3& mat3f) const = 0;
		virtual void UploadMatrix4f(const std::string& name, const glm::mat4& mat4f) const = 0;

	public:
		static IGRef<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static IGRef<Shader> Create(const std::string& filepath);
	};

	class ShaderLibrary {
	public:
		void Add(const IGRef<Shader>& shader);
		void Add(const std::string& name, const IGRef<Shader>& shader);

		IGRef<Shader> Load(const std::string& filepath);
		IGRef<Shader> Load(const std::string& name, const std::string& filepath);

		IGRef<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, IGRef<Shader>> mShaders;
	};
}