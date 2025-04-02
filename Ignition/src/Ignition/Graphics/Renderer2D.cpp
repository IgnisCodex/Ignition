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

		glm::vec4 QuadVertexPositions[4];

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

		sData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		sData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		sData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		sData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Terminate() {
		delete[] sData.QuadVertexBufferBase;
	}

	void Renderer2D::SceneBegin(const Camera& camera, const glm::mat4& transform) {
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);
		
		sData.Default2DShader->Bind();
		sData.Default2DShader->UploadMatrix4f("u_ViewProjection", viewProjection);

		sData.QuadIndexCount = 0;
		sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;
		sData.TextureSlotIndex = 1;
	}

	void Renderer2D::SceneBegin(const OrthoCamera& camera) {
		sData.Default2DShader->Bind();
		sData.Default2DShader->UploadMatrix4f("u_ViewProjection", camera.GetViewProjectionMatrix());

		sData.QuadIndexCount = 0;
		sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;
		sData.TextureSlotIndex = 1;
	}

	void Renderer2D::SceneEnd() {
		uint32_t size = (uint32_t)((uint8_t*)sData.QuadVertexBufferPtr - (uint8_t*)sData.QuadVertexBufferBase);
		sData.QuadVertexBuffer->SetData(sData.QuadVertexBufferBase, size);

		Flush();
	}

	void Renderer2D::Flush() {
		for (uint32_t i = 0; i < sData.TextureSlotIndex; i++) {
			sData.TextureSlots[i]->Bind(i);
		}

		RenderCall::DrawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);
	}


	// ==== Draw Quad ==================================

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour) {
		DrawQuad({ position.x, position.y }, 0.0f, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const IGRef<Texture2D>& texture, const glm::vec4& tint) {
		DrawQuad({ position.x, position.y }, 0.0f, size, texture, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const IGRef<SubTexture2D>& subtexture, const glm::vec4& tint) {
		DrawQuad(position, 0.0f, size, subtexture, tint);
	}


	// ==== Draw Quad with Z-Index =====================

	void Renderer2D::DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const glm::vec4& colour) {
		DrawQuad({position.x, position.y, zIndex}, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const IGRef<Texture2D>& texture, const glm::vec4& tint) {
		DrawQuad({ position.x, position.y, zIndex }, size, texture, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const IGRef<SubTexture2D>& subtexture, const glm::vec4& tint) {
		DrawQuad({ position.x, position.y, zIndex }, size, subtexture, tint);
	}


	// ==== Draw Quad with Z-Index using a vec3 =========

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour) {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const IGRef<Texture2D>& texture, const glm::vec4& tint) {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const IGRef<SubTexture2D>& subtexture, const glm::vec4& tint) {
		constexpr size_t quadVertexCount = 4;
		//constexpr glm::vec4 colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		const IGRef<Texture2D> texture = subtexture->GetTexture();
		const glm::vec2* textureCoordinates = subtexture->GetTextureCoordinates();

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

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++) {
			sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVertexBufferPtr->Colour = tint;
			sData.QuadVertexBufferPtr->TextureCoordinates = textureCoordinates[i];
			sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
			//s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			sData.QuadVertexBufferPtr++;
		}

		sData.QuadIndexCount += 6;
	}


	// ==== Draw Quad using a Transform Matrix

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& colour) {
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		//if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		//	FlushAndReset();

		for (size_t i = 0; i < quadVertexCount; i++) {
			sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVertexBufferPtr->Colour = colour;
			sData.QuadVertexBufferPtr->TextureCoordinates = textureCoords[i];
			sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
			//sData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			sData.QuadVertexBufferPtr++;
		}

		sData.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const IGRef<Texture2D>& texture, const glm::vec4& tint) {

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		//if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		//	FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < sData.TextureSlotIndex; i++) {
			if (*sData.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			/*if (sData.TextureSlotIndex >= Data::MaxTextureSlots)
				FlushAndReset();*/

			textureIndex = (float)sData.TextureSlotIndex;
			sData.TextureSlots[sData.TextureSlotIndex] = texture;
			sData.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[i];
			sData.QuadVertexBufferPtr->Colour = tint;
			sData.QuadVertexBufferPtr->TextureCoordinates = textureCoords[i];
			sData.QuadVertexBufferPtr->TextureIndex = textureIndex;
			//sData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			sData.QuadVertexBufferPtr++;
		}

		sData.QuadIndexCount += 6;
	}
}