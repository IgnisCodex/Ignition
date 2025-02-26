#include "IGPCH.hpp"

#include "Ignition/Core/Application.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Events/Event.hpp"
#include "Ignition/UI/ImGuiLayer.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

//#include <glad/glad.h>

namespace Ignition::Core {

	Application* Application::sInstance = nullptr;

	Application::Application() {
		IG_CORE_ASSERT(!sInstance, "Application Instance Already Exists!");
		sInstance = this;

		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		mImGuiLayer = new UI::ImGuiLayer();
		PushOverlay(mImGuiLayer);

		glGenVertexArrays(1, &mVertexArray);
		glBindVertexArray(mVertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f, 1.0f
		};

		mVertexBuffer.reset(Graphics::VertexBuffer::Create(vertices, sizeof(vertices)));
		
		
		{
			Graphics::BufferLayout layout = {
				{ Graphics::DataType::Vector3f,	"a_Position"	},
				{ Graphics::DataType::Vector4f,	"a_Colour"		}
			};

			mVertexBuffer->SetLayout(layout);
		}

		const auto& layout = mVertexBuffer->GetLayout();

		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				Backends::DataTypetoOpenGLBaseType(element.Type),
				element.Normalised ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			IG_CORE_TRACE("glVertexAttribPointer(index = {}, count = {}, type = 0x{:04x}, norm = {}, stride = {}, offset = {})", index,
				element.GetComponentCount(),
				Backends::DataTypetoOpenGLBaseType(element.Type),
				element.Normalised, layout.GetStride(), element.Offset);
			index++;
		}

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t indices[3] = { 0, 1, 2 };
		mIndexBuffer.reset(Graphics::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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

		while (mIsRunning) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			mShader->Bind();
			glBindVertexArray(mVertexArray);
			glDrawElements(GL_TRIANGLES, mIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
