#include "IGPCH.hpp"

#include "Ignition/Core/Application.hpp"

#include "Ignition/API.hpp"
#include "Ignition/Log.hpp"
#include "Ignition/Events/Event.hpp"
#include "Ignition/Graphics/RenderCall.hpp"
#include "Ignition/Graphics/Renderer.hpp"
#include "Ignition/UI/ImGuiLayer.hpp"
#include "Ignition/Util/Time.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

#include <GLFW/glfw3.h>

namespace Ignition::Core {

	Application* Application::sInstance = nullptr;

	Application::Application(const std::string& name) {
		IG_CORE_ASSERT(!sInstance, "Application Instance Already Exists!");
		sInstance = this;

		mWindow = Window::Create(WindowProperties(name));
		mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Graphics::Renderer::Init();

		mImGuiLayer = new UI::ImGuiLayer();
		PushOverlay(mImGuiLayer);
	}

	Application::~Application() {}

	void Application::Run() {
		IG_CORE_INFO("Ignition Engine v0.1");

		while (mIsRunning) {

			float time = (float)glfwGetTime();
			Util::DeltaTime dt = time - mPreviousFrameTime;
			mPreviousFrameTime = time;

			if (!mIsMinimized) {
				for (Core::Layer* layer : mLayerStack)
					layer->OnUpdate(dt);
			}

			mImGuiLayer->Begin();
			for (Core::Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->End();

			mWindow->OnUpdate();
		}
	}

	void Application::Quit() {
		mIsRunning = false;
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
		dispatcher.Dispatch<Events::WindowCloseEvent>(IG_BIND_EVENT(Application::OnWindowCloseEvent));
		dispatcher.Dispatch<Events::WindowResizeEvent>(IG_BIND_EVENT(Application::OnWindowResize));

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

	bool Application::OnWindowResize(Events::WindowResizeEvent& event) {
		if (event.GetWidth() == 0 || event.GetHeight() == 0) {
			mIsMinimized = true;
			return false;
		}
		
		Graphics::Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

		mIsMinimized = false;
		return false;
	}
}
