#include <Ignition.hpp>

class ExampleLayer : public Ignition::Core::Layer {
public:
	ExampleLayer()
		: mOrthoCamera(-1.6f, 1.6f, -0.9f, 0.9f)
		, Layer("Example")
	{
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f, 1.0f
		};

		mVertexBuffer.reset(Ignition::Graphics::VertexBuffer::Create(vertices, sizeof(vertices)));
		Ignition::Graphics::BufferLayout layout = {
			{ Ignition::Graphics::DataType::Vector3f,	"a_Position"	},
			{ Ignition::Graphics::DataType::Vector4f,	"a_Colour"		}
		};
		mVertexBuffer->SetLayout(layout);

		mVertexArray.reset(Ignition::Graphics::VertexArray::Create());
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		mIndexBuffer.reset(Ignition::Graphics::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->SetIndexBuffer(mIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Position = a_Position;
				v_Colour = a_Colour;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
				
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Colour;

			void main()
			{
				color = vec4(v_Position + 0.5, 1.0);
				color = v_Colour;
			}
		)";

		mShader.reset(Ignition::Graphics::Shader::Create(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override {
		mOrthoCamera.SetPosition({ 0.5f, 0.5f, 0.0f });
		mOrthoCamera.SetRotation(35);

		if (Ignition::Graphics::Renderer::SceneBegin(mOrthoCamera)) {
			//mShader->Bind();
			//mShader->UploadMatrix4f("u_ViewProjection", mOrthoCamera.GetViewProjectionMatrix());
			Ignition::Graphics::Renderer::Submit(mShader, mVertexArray);

			Ignition::Graphics::Renderer::SceneEnd();
		}
	}

	void OnEvent(Ignition::Events::Event& event) override {
		//IG_TRACE("{}", event.GetName());
	}

private:
	std::shared_ptr<Ignition::Graphics::VertexBuffer> mVertexBuffer;
	std::shared_ptr<Ignition::Graphics::VertexArray> mVertexArray;
	std::shared_ptr<Ignition::Graphics::IndexBuffer> mIndexBuffer;
	std::shared_ptr<Ignition::Graphics::Shader> mShader;

	Ignition::Graphics::OrthoCamera mOrthoCamera;
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