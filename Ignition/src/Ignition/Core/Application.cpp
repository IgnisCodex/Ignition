#include "IGPCH.hpp"

#include "Ignition/Core/Application.hpp"

#include "Ignition/Log.hpp"
#include "Ignition/Events/Event.hpp"
#include "Ignition/Graphics/RenderCall.hpp"
#include "Ignition/Graphics/Renderer.hpp"
#include "Ignition/UI/ImGuiLayer.hpp"
#include "Ignition/Util/Time.hpp"
#include "Ignition/Util/Util.hpp"

#include "Backends/OpenGL/OpenGLShader.hpp"

#include <GLFW/glfw3.h>

namespace Ignition::Core {

	Application* Application::sInstance = nullptr;

	Application::Application() {
		IG_CORE_ASSERT(!sInstance, "Application Instance Already Exists!");
		sInstance = this;

		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		mImGuiLayer = new UI::ImGuiLayer();
		PushOverlay(mImGuiLayer);
	}

	Application::~Application() {}

	void Application::Run() {
		IG_CORE_INFO("Ignition Engine v0.1");

		Graphics::RenderCall::Clear(rgb(65, 65, 65));

		while (mIsRunning) {

			float time = (float)glfwGetTime();
			Util::DeltaTime dt = time - mPreviousFrameTime;
			mPreviousFrameTime = time;


			Graphics::RenderCall::Clear();

			for (Core::Layer* layer : mLayerStack)
				layer->OnUpdate(dt);

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
