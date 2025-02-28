#pragma once

#include "IGPCH.hpp"

#include "Ignition/API.hpp"
#include "Ignition/Core/Window.hpp"
#include "Ignition/Core/LayerStack.hpp"
#include "Ignition/Events/Event.hpp"
#include "Ignition/Events/ApplicationEvent.hpp"
#include "Ignition/Graphics/Buffers.hpp"
#include "Ignition/Graphics/Camera.hpp"
#include "Ignition/Graphics/Shader.hpp"
#include "Ignition/Graphics/VertexArray.hpp"
#include "Ignition/UI/ImGuiLayer.hpp"

namespace Ignition::Core {
	class IGNITION_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Events::Event& event);

		void PushLayer(Core::Layer* layer);
		void PushOverlay(Core::Layer* layer);

		inline Window& GetWindow() { return *mWindow; }
		inline static Application& Get() { return *sInstance; }

	private:
		bool OnWindowCloseEvent(Events::WindowCloseEvent& event);

	private:
		std::unique_ptr<Window> mWindow;

		UI::ImGuiLayer* mImGuiLayer;
		Core::LayerStack mLayerStack;

		bool mIsRunning = true;
		bool mIsMinimized = false;

	private:
		static Application* sInstance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
