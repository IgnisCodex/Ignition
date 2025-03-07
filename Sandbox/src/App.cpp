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

		std::string flatColourVertexShaderSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
				
			}
		)";

		std::string flatColourFragmentShaderSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec4 u_Colour;

			in vec3 v_Position;
			in vec4 v_Colour;

			void main()
			{
				color = u_Colour;
			}
		)";

		mFlatColourShader.reset(Ignition::Graphics::Shader::Create(flatColourVertexShaderSrc, flatColourFragmentShaderSrc));


		std::string textureVertexShaderSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoords;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoords;

			void main()
			{
				v_TexCoords = a_TexCoords;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
				
			}
		)";

		std::string textureFragmentShaderSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			uniform sampler2D u_Texture;

			in vec2 v_TexCoords;

			void main()
			{
				color = texture(u_Texture, v_TexCoords);
			}
		)";

		mTextureShader.reset(Ignition::Graphics::Shader::Create(textureVertexShaderSrc, textureFragmentShaderSrc));

		mTexture = Ignition::Graphics::Texture2D::Create("assets/textures/band.png");

		mTextureShader->Bind();
		mTextureShader->UploadInt("u_Texture", 0);
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

			mTexture->Bind();
			Ignition::Graphics::Renderer::Submit(mTextureShader, mVertexArray);

			Ignition::Graphics::Renderer::SceneEnd();
		}
	}

	void OnEvent(Ignition::Events::Event& event) override {
		
	}

private:
	Ignition::Ref<Ignition::Graphics::VertexBuffer> mVertexBuffer;
	Ignition::Ref<Ignition::Graphics::VertexArray> mVertexArray;
	Ignition::Ref<Ignition::Graphics::IndexBuffer> mIndexBuffer;
	Ignition::Ref<Ignition::Graphics::Shader> mFlatColourShader, mTextureShader;

	Ignition::Ref<Ignition::Graphics::Texture2D> mTexture;

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