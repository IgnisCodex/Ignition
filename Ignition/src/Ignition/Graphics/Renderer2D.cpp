#include "IGPCH.hpp"
#include "Ignition/Graphics/Renderer2D.hpp"

#include "Ignition/Graphics/RenderCall.hpp"
#include "Ignition/Graphics/Shader.hpp"
#include "Ignition/Graphics/VertexArray.hpp"

namespace Ignition::Graphics {
	struct Data {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColourShader;
	};

	static Data* sData;

	void Renderer2D::Init() {
		sData = new Data();

		sData->QuadVertexArray = VertexArray::Create();

		float squareVerticies[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVerticies, sizeof(squareVerticies));

		squareVB->SetLayout({
			{ DataType::Vector3f, "a_Position" }
			});
		sData->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t));
		sData->QuadVertexArray->SetIndexBuffer(squareIB);

		sData->FlatColourShader = Shader::Create("assets/shaders/FlatColour.glsl");
	}

	void Renderer2D::Terminate() {
		delete sData;
	}

	void Renderer2D::SceneBegin(const OrthoCamera& camera) {
		sData->FlatColourShader->Bind();
		sData->FlatColourShader->UploadMatrix4f("u_ViewProjection", camera.GetViewProjectionMatrix());
		sData->FlatColourShader->UploadMatrix4f("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::SceneEnd() {
	
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour) {
		DrawQuad(position, 0.0f, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float zIndex, const glm::vec2& size, const glm::vec4& colour) {
		sData->FlatColourShader->Bind();
		sData->FlatColourShader->UploadVector4f("u_Colour", colour);

		sData->QuadVertexArray->Bind();
		RenderCall::DrawIndexed(sData->QuadVertexArray);
	}

}