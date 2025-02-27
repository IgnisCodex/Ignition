#include "IGPCH.hpp"

#include "Ignition/Core/Application.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Events/Event.hpp"
#include "Ignition/Graphics/RenderCall.hpp"
#include "Ignition/Graphics/Renderer.hpp"
#include "Ignition/UI/ImGuiLayer.hpp"
#include "Ignition/Util/Util.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

namespace Ignition::Core {

	Application* Application::sInstance = nullptr;

	Application::Application() {
		IG_CORE_ASSERT(!sInstance, "Application Instance Already Exists!");
		sInstance = this;

		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		mImGuiLayer = new UI::ImGuiLayer();
		PushOverlay(mImGuiLayer);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f, 1.0f
		};

		mVertexBuffer.reset(Graphics::VertexBuffer::Create(vertices, sizeof(vertices)));
		Graphics::BufferLayout layout = {
			{ Graphics::DataType::Vector3f,	"a_Position"	},
			{ Graphics::DataType::Vector4f,	"a_Colour"		}
		};
		mVertexBuffer->SetLayout(layout);

		mVertexArray.reset(Graphics::VertexArray::Create());
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		mIndexBuffer.reset(Graphics::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->SetIndexBuffer(mIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Position = a_Position;
				v_Colour = a_Colour;
				gl_Position = vec4(a_Position, 1.0);	
				
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

		mShader.reset(Graphics::Shader::Create(vertexSrc, fragmentSrc));
	}

	Application::~Application() {}

	void Application::Run() {
		IG_CORE_INFO("Started!");

		Graphics::RenderCall::Clear(rgb(96, 71, 129));

		while (mIsRunning) {

			Graphics::RenderCall::Clear();

			if (Graphics::Renderer::SceneBegin()) {
				mShader->Bind();
                Graphics::Renderer::Submit(mVertexArray);

				Graphics::Renderer::SceneEnd();
			}

			for (Core::Layer* layer : mLayerStack)
				layer->OnUpdate();

			mImGuiLayer->Begin();
			for (Core::Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->End();

			mWindow->OnUpdate();
		}
	}

	void Application::PushLayer(Core::Layer* layer) {
		mLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Core::Layer* layer) {
		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Events::Event& event) {
		Events::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Events::WindowCloseEvent>(std::bind(&Application::OnWindowCloseEvent, this, std::placeholders::_1));

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.IsHandled)
				break;
		}
	}

	// Private Methods
	bool Application::OnWindowCloseEvent(Events::WindowCloseEvent& event) {
		mIsRunning = false;
		return true;
	}
}
