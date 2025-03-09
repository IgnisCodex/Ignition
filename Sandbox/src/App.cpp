#include <Ignition.hpp>
#include <Ignition/Util/Util.hpp>

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Ignition::Core::Layer {
public:
	ExampleLayer()
		: Layer("Example")
		, mOrthoCameraContr(1280.0f / 720.0f)
	{
		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		mVertexBuffer.reset(Ignition::Graphics::VertexBuffer::Create(vertices, sizeof(vertices)));
		Ignition::Graphics::BufferLayout layout = {
			{ Ignition::Graphics::DataType::Vector3f,	"a_Position"	},
			{ Ignition::Graphics::DataType::Vector2f,	"a_TexCoords"	}
		};
		mVertexBuffer->SetLayout(layout);

		mVertexArray.reset(Ignition::Graphics::VertexArray::Create());
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		mIndexBuffer.reset(Ignition::Graphics::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->SetIndexBuffer(mIndexBuffer);

		auto textureShader = mShaderLibrary.Load("assets/shaders/texture.glsl");

		mTexture = Ignition::Graphics::Texture2D::Create("assets/textures/band.png");

		textureShader->Bind();
		textureShader->UploadInt("u_Texture", 0);
	}

	void OnUpdate(Ignition::Util::DeltaTime dt) override {

		mOrthoCameraContr.OnUpdate(dt);

		if (Ignition::Graphics::Renderer::SceneBegin(mOrthoCameraContr.GetCamera())) {
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

			auto textureShader = mShaderLibrary.Get("texture");
			mTexture->Bind();
			Ignition::Graphics::Renderer::Submit(textureShader, mVertexArray);

			Ignition::Graphics::Renderer::SceneEnd();
		}
	}

	void OnEvent(Ignition::Events::Event& event) override {
		mOrthoCameraContr.OnEvent(event);
	}

private:
	Ignition::Graphics::ShaderLibrary mShaderLibrary;

	Ignition::Ref<Ignition::Graphics::VertexBuffer> mVertexBuffer;
	Ignition::Ref<Ignition::Graphics::VertexArray> mVertexArray;
	Ignition::Ref<Ignition::Graphics::IndexBuffer> mIndexBuffer;

	Ignition::Ref<Ignition::Graphics::Texture2D> mTexture;

	Ignition::Graphics::OrthoCameraContr mOrthoCameraContr;
};

class Sandbox : public Ignition::Core::Application {
public:
	Sandbox() {
		IG_INFO("Started!");
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Ignition::Core::Application* Ignition::Core::CreateApplication() {
	return new Sandbox();
}