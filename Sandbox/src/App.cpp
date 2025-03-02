#include <Ignition.hpp>
#include <Ignition/Util/Util.hpp>

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Ignition::Core::Layer {
public:
	ExampleLayer()
		: mOrthoCamera(-1.6f, 1.6f, -0.9f, 0.9f)
		, Layer("Example")
		, mOrthoCameraPosition(0.0f)
		, mTriPosition(0.0f)
	{
		IGColour colour1(rgb(26, 0, 165));
		IGColour colour4(rgb(30, 187, 22));
		IGColour colour3(rgb(252, 222, 10));
		IGColour colour2(rgb(240, 0, 22));

		float vertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f,		colour1.r, colour1.g, colour1.b, 1.0f,
			 0.5f, -0.5f, 0.0f,		colour2.r, colour2.g, colour2.b, 1.0f,
			 0.5f,  0.5f, 0.0f,		colour3.r, colour3.g, colour3.b, 1.0f,
			-0.5f,  0.5f, 0.0f,		colour4.r, colour4.g, colour4.b, 1.0f
		};

		mVertexBuffer.reset(Ignition::Graphics::VertexBuffer::Create(vertices, sizeof(vertices)));
		Ignition::Graphics::BufferLayout layout = {
			{ Ignition::Graphics::DataType::Vector3f,	"a_Position"	},
			{ Ignition::Graphics::DataType::Vector4f,	"a_Colour"		}
		};
		mVertexBuffer->SetLayout(layout);

		mVertexArray.reset(Ignition::Graphics::VertexArray::Create());
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		mIndexBuffer.reset(Ignition::Graphics::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->SetIndexBuffer(mIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Position = a_Position;
				v_Colour = a_Colour;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
				
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

	void OnUpdate(Ignition::Util::DeltaTime dt) override {

		if (Ignition::Core::Input::IsKeyPressed(IG_KEY_W)) {
			mOrthoCameraPosition.y += mOrthoCameraSpeed * dt.s();
		
		} else if (Ignition::Core::Input::IsKeyPressed(IG_KEY_S)) {
			mOrthoCameraPosition.y -= mOrthoCameraSpeed * dt.s();
		}
		
		if (Ignition::Core::Input::IsKeyPressed(IG_KEY_D)) {
			mOrthoCameraPosition.x += mOrthoCameraSpeed * dt.s();
		
		} else if (Ignition::Core::Input::IsKeyPressed(IG_KEY_A)) {
			mOrthoCameraPosition.x -= mOrthoCameraSpeed * dt.s();
		}

		mOrthoCamera.SetPosition(mOrthoCameraPosition);

		if (Ignition::Graphics::Renderer::SceneBegin(mOrthoCamera)) {
			//mShader->Bind();
			//mShader->UploadMatrix4f("u_ViewProjection", mOrthoCamera.GetViewProjectionMatrix());

			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

			for (int y = 0; y < 20; y++) {
				for (int x = 0; x < 20; x++) {
					glm::vec3 pos(x * 0.51f, y * 0.51f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
					Ignition::Graphics::Renderer::Submit(mShader, mVertexArray, transform);
				}
			}

			Ignition::Graphics::Renderer::SceneEnd();
		}
	}

	void OnEvent(Ignition::Events::Event& event) override {
		
	}

private:
	std::shared_ptr<Ignition::Graphics::VertexBuffer> mVertexBuffer;
	std::shared_ptr<Ignition::Graphics::VertexArray> mVertexArray;
	std::shared_ptr<Ignition::Graphics::IndexBuffer> mIndexBuffer;
	std::shared_ptr<Ignition::Graphics::Shader> mShader;

	Ignition::Graphics::OrthoCamera mOrthoCamera;
	glm::vec3 mOrthoCameraPosition;
	float mOrthoCameraSpeed = 3.0f;

	glm::vec3 mTriPosition;
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