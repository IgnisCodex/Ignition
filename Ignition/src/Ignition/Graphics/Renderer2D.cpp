#include "IGPCH.hpp"
#include "Ignition/Graphics/Renderer2D.hpp"

#include "Ignition/API.hpp"

#include "Ignition/Graphics/RenderCall.hpp"
#include "Ignition/Graphics/Shader.hpp"
#include "Ignition/Graphics/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ignition::Graphics {
	struct Data {
		IGRef<VertexArray> QuadVertexArray;
		IGRef<Shader> Default2DShader;
		IGRef<Texture2D> DefaultTexture;
	};

	static Data* sData;

	void Renderer2D::Init() {
		sData = new Data();

		sData->QuadVertexArray = VertexArray::Create();

		float squareVerticies[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		IGRef<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVerticies, sizeof(squareVerticies));

		squareVB->SetLayout({
			{ DataType::Vector3f, "a_Position" },
			{ DataType::Vector2f, "a_TextureCoords" }
		});
		sData->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		IGRef<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t));
		sData->QuadVertexArray->SetIndexBuffer(squareIB);

		sData->DefaultTexture = Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		sData->DefaultTexture->SetData(&textureData, sizeof(textureData));

		sData->Default2DShader = Shader::Create("assets/shaders/Default2D.glsl");
		sData->Default2DShader->Bind();
		sData->Default2DShader->UploadInt("u_Texture", 0);
	}

	void Renderer2D::Terminate() {
		delete sData;
	}

	void Renderer2D::SceneBegin(const OrthoCamera& camera) {
		sData->Default2DShader->Bind();
		sData->Default2DShader->UploadMatrix4f("u_ViewProjection", camera.GetViewProjectionMatrix());
		sData->Default2DShader->UploadMatrix4f("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::SceneEnd() {
	
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour) {
		DrawQuad(position, 0.0f, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const glm::vec4& colour) {
		sData->Default2DShader->Bind();
		sData->Default2DShader->UploadVector4f("u_Colour", colour);

		sData->DefaultTexture->Bind();

		// Translation -> Rotation -> Scale

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, zIndex })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 0 });
		sData->Default2DShader->UploadMatrix4f("u_Transform", transform);
		
		sData->QuadVertexArray->Bind();
		RenderCall::DrawIndexed(sData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const IGRef<Texture2D>& texture) {
		DrawQuad(position, 0.0f, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const IGRef<Texture2D>& texture) {
		sData->Default2DShader->Bind();
		sData->Default2DShader->UploadVector4f("u_Colour", rgba(255, 255, 255, 1));

		texture->Bind();

		// Translation -> Rotation -> Scale

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, zIndex })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 0 });
		sData->Default2DShader->UploadMatrix4f("u_Transform", transform);

		sData->QuadVertexArray->Bind();
		RenderCall::DrawIndexed(sData->QuadVertexArray);
	}

}