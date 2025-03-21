#include "IGPCH.hpp"
#include "Ignition/Graphics/Renderer2D.hpp"

#include "Ignition/API.hpp"

#include "Ignition/Graphics/RenderCall.hpp"
#include "Ignition/Graphics/Shader.hpp"
#include "Ignition/Graphics/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ignition::Graphics {
	struct QuadVertex {
		glm::vec3 Position;
		IGColour Colour;
		glm::vec2 TextureCoordinates;
		float TextureIndex;
	};

	struct Data {
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndicies = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;				// TODO: Grab this from GPU

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		IGRef<VertexBuffer> QuadVertexBuffer;
		IGRef<VertexArray> QuadVertexArray;
		IGRef<Shader> Default2DShader;
		IGRef<Texture2D> DefaultTexture;

		std::array<IGRef<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;							// Slot 0 is reserved for the default texture
	};

	static Data sData;

	void Renderer2D::Init() {

		sData.QuadVertexArray = VertexArray::Create();

		sData.QuadVertexBuffer = VertexBuffer::Create(sData.MaxVertices * sizeof(QuadVertex));
		sData.QuadVertexBuffer->SetLayout({
			{ DataType::Vector3f,	"a_Position" },
			{ DataType::Vector4f,	"a_Colour" },
			{ DataType::Vector2f,	"a_TextureCoordinates" },
			{ DataType::Float,		"a_TextureIndex" }
		});
		sData.QuadVertexArray->AddVertexBuffer(sData.QuadVertexBuffer);

		sData.QuadVertexBufferBase = new QuadVertex[sData.MaxVertices];

		uint32_t* quadIndicies = new uint32_t[sData.MaxIndicies];
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < sData.MaxIndicies; i += 6) {
			quadIndicies[i + 0] = offset + 0;
			quadIndicies[i + 1] = offset + 1;
			quadIndicies[i + 2] = offset + 2;

			quadIndicies[i + 3] = offset + 2;
			quadIndicies[i + 4] = offset + 3;
			quadIndicies[i + 5] = offset + 0;

			offset += 4;
		}

		IGRef<IndexBuffer> quadIB = IndexBuffer::Create(quadIndicies, sData.MaxIndicies);
		sData.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndicies;

		sData.DefaultTexture = Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		sData.DefaultTexture->SetData(&textureData, sizeof(uint32_t));

		int32_t samplers[sData.MaxTextureSlots];
		for (uint32_t i = 0; i < sData.MaxTextureSlots; i++)
			samplers[i] = i;

		sData.Default2DShader = Shader::Create("assets/shaders/Default2D.glsl");
		sData.Default2DShader->Bind();
		sData.Default2DShader->UploadIntArray("u_Textures", samplers, sData.MaxTextureSlots);

		sData.TextureSlots[0] = sData.DefaultTexture;
	}

	void Renderer2D::Terminate() {
		
	}

	void Renderer2D::SceneBegin(const OrthoCamera& camera) {
		sData.Default2DShader->Bind();
		sData.Default2DShader->UploadMatrix4f("u_ViewProjection", camera.GetViewProjectionMatrix());
		
		sData.QuadIndexCount = 0;
		sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;
		sData.TextureSlotIndex = 1;
	}

	void Renderer2D::SceneEnd() {
		uint32_t size = (uint8_t*)sData.QuadVertexBufferPtr - (uint8_t*)sData.QuadVertexBufferBase;
		sData.QuadVertexBuffer->SetData(sData.QuadVertexBufferBase, size);

		Flush();
	}

	void Renderer2D::Flush() {
		for (uint32_t i = 0; i < sData.TextureSlotIndex; i++) {
			sData.TextureSlots[i]->Bind(i);
		}

		RenderCall::DrawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour) {
		DrawQuad(position, 0.0f, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const glm::vec4& colour) {
		sData.Default2DShader->Bind();
		// This sets the texture index to the default texture which is at slot 0.
		const float textureIndex = 0.0f;
		
		sData.QuadVertexBufferPtr->Position = { position.x, position.y, zIndex };
		sData.QuadVertexBufferPtr->Colour = colour;
		sData.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 0.0f };
		sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, zIndex };
		sData.QuadVertexBufferPtr->Colour = colour;
		sData.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 0.0f };
		sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, zIndex };
		sData.QuadVertexBufferPtr->Colour = colour;
		sData.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 1.0f };
		sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, zIndex };
		sData.QuadVertexBufferPtr->Colour = colour;
		sData.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 1.0f };
		sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		sData.QuadVertexBufferPtr++;

		sData.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const IGRef<Texture2D>& texture, const glm::vec4& tint) {
		DrawQuad(position, 0.0f, size, texture, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const IGRef<Texture2D>& texture, const glm::vec4& tint) {
		sData.Default2DShader->Bind();
		constexpr glm::vec4 colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < sData.TextureSlotIndex; i++) {
			if (*sData.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = texture;
			sData.TextureSlotIndex++;
		}
		
		sData.QuadVertexBufferPtr->Position = { position.x, position.y, zIndex };
		sData.QuadVertexBufferPtr->Colour = colour;
		sData.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 0.0f };
		sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, zIndex };
		sData.QuadVertexBufferPtr->Colour = colour;
		sData.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 0.0f };
		sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, zIndex };
		sData.QuadVertexBufferPtr->Colour = colour;
		sData.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 1.0f };
		sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		sData.QuadVertexBufferPtr++;

		sData.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, zIndex };
		sData.QuadVertexBufferPtr->Colour = colour;
		sData.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 1.0f };
		sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
		sData.QuadVertexBufferPtr++;

		sData.QuadIndexCount += 6;
	}


}