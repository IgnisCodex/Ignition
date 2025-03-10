#include "Sandbox2D.hpp"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
	, mCameraContr(1280.0f / 720.0f)
{}

void Sandbox2D::OnAttach() {
	mSquareVA = Ignition::Graphics::VertexArray::Create();

	float squareVerticies[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Ignition::Ref<Ignition::Graphics::VertexBuffer> squareVB;
	squareVB = Ignition::Graphics::VertexBuffer::Create(squareVerticies, sizeof(squareVerticies));

	squareVB->SetLayout({
		{ Ignition::Graphics::DataType::Vector3f, "a_Position" }
	});
	mSquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
	Ignition::Ref<Ignition::Graphics::IndexBuffer> squareIB;
	squareIB = Ignition::Graphics::IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t));
	mSquareVA->SetIndexBuffer(squareIB);

	mFlatColourShader = Ignition::Graphics::Shader::Create("assets/shaders/FlatColour.glsl");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Ignition::Util::DeltaTime dt) {

	Ignition::Graphics::RenderCall::Clear(rgb(15, 23, 42));

	mCameraContr.OnUpdate(dt);

	if (Ignition::Graphics::Renderer::SceneBegin(mCameraContr.GetCamera())) {
		mFlatColourShader->Bind();
		mFlatColourShader->UploadVector4f("u_Colour", mSquareColour);

		Ignition::Graphics::Renderer::Submit(mFlatColourShader, mSquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Ignition::Graphics::Renderer::SceneEnd();
	}
}

void Sandbox2D::OnImGuiRender() {
	if (ImGui::Begin("Settings")) {
		ImGui::ColorEdit4("Square Colour", glm::value_ptr(mSquareColour));
		ImGui::End();
	}
}

void Sandbox2D::OnEvent(Ignition::Events::Event& event) {
	mCameraContr.OnEvent(event);
}