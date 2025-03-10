#pragma once

#include "Ignition/Graphics/Shader.hpp"

namespace Ignition::Graphics {

	struct BufferElement {
		DataType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool Normalised;

		BufferElement()
			: Type(DataType::Float), Name("Default"), Size(GetDataTypeSize(DataType::Float)), Offset(0), Normalised(false)
		{}

		BufferElement(DataType type, std::string name, bool normalised = false)
			: Type(type), Name(name), Size(GetDataTypeSize(type)), Offset(0), Normalised(normalised)
		{}

		uint32_t GetComponentCount() const {
			switch (Type) {
			case DataType::Bool:
				return 1;

			case DataType::Float:
				return 1;
			case DataType::Vector2f:
				return 2;
			case DataType::Vector3f:
				return 3;
			case DataType::Vector4f:
				return 4;

			case DataType::Int:
				return 1;
			case DataType::Vector2i:
				return 2;
			case DataType::Vector3i:
				return 3;
			case DataType::Vector4i:
				return 4;

			case DataType::Matrix3f:
				return 3 * 3;
			case DataType::Matrix4f:
				return 4 * 4;
			}

			IG_CORE_ASSERT(false, "Invalid Data Type!");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: mElements(elements)
		{
			// Calculates the Stride and Offset of each element
			CalculateLayout();
		}

		inline uint32_t GetStride() const { return mStride; }
		inline const std::vector<BufferElement>& GetElements() const { return mElements; }

		std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		std::vector<BufferElement>::iterator end() { return mElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

	private:
		inline void CalculateLayout() {
			uint32_t offset = 0;
			mStride = 0;
			for (auto& element : mElements) {
				element.Offset = offset;
				offset += element.Size;
				mStride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> mElements;
		uint32_t mStride = 0;
	};
	
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

	public:
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

	public:
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}