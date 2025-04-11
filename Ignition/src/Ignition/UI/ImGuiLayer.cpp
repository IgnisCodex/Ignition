#include "IGPCH.hpp"

#include "Ignition/UI/ImGuiLayer.hpp"

#include "Ignition/Core/Application.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <libs/implot/implot.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Ignition::UI {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::OnAttach() {
		// Setup ImGui Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


		static ImFont* font = NULL;
		font = io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/roboto-var.ttf", 16.0f);
		io.FontDefault = font;

		SetStyle();

		Core::Application& app = Core::Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() {
		ImGuiIO& io = ImGui::GetIO();
		Core::Application& app = Core::Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetStyle() {
		
		ImGuiStyle& style = ImGui::GetStyle();

		// Rounding
		style.FrameRounding = 5.0f;
		style.PopupRounding = 5.0f;
		style.ChildRounding = 5.0f;
		style.GrabRounding = 5.0f;
		style.TabRounding = 5.0f;
		style.WindowRounding = 5.0f;
		
		// Other
		style.DockingSeparatorSize = 1.0f;
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.ColorButtonPosition = ImGuiDir_Left;
		style.WindowTitleAlign = { 0.5f, 0.5f };
		
		SetDarkTheme();
	}

	void ImGuiLayer::SetDarkTheme() {
		auto& colours = ImGui::GetStyle().Colors;

		glm::vec4 windows = rgb(35, 40, 42);

		glm::vec4 header = rgb(45, 50, 52);
		glm::vec4 headerActive = header;
		glm::vec4 headerHovered = rgb(55, 60, 62);
		
		glm::vec4 button = rgb(45, 50, 52);
		glm::vec4 buttonActive = button;
		glm::vec4 buttonHovered = rgb(55, 60, 62);

		glm::vec4 frame = rgb(45, 50, 52);
		glm::vec4 frameActive = button;
		glm::vec4 frameHovered = rgb(55, 60, 62);

		glm::vec4 tab = rgb(45, 50, 52);
		glm::vec4 tabActive = rgb(55, 60, 62);
		glm::vec4 tabHovered = tabActive;
		glm::vec4 tabUnfocused = rgba(0, 0, 0, 0);
		glm::vec4 tabUnfocusedHovered = tabHovered;
		glm::vec4 tabSelectedOverline = rgb(65, 90, 82);

		glm::vec4 title = rgb(35, 40, 42);
		glm::vec4 titleActive = rgb(45, 50, 52);
		glm::vec4 titleCollapsed = title;

		// Windows
		colours[ImGuiCol_WindowBg] = ImVec4{ windows.r, windows.g, windows.b, windows.a };
		
		// Headers
		colours[ImGuiCol_Header] = ImVec4{ header.r, header.g, header.b, header.a };
		colours[ImGuiCol_HeaderActive] = ImVec4{ headerActive.r, headerActive.g, headerActive.b, headerActive.a };
		colours[ImGuiCol_HeaderHovered] = ImVec4{ headerHovered.r, headerHovered.g, headerHovered.b, headerHovered.a };

		// Buttons
		colours[ImGuiCol_Button] = ImVec4{ button.r, button.g, button.b, button.a, };
		colours[ImGuiCol_ButtonActive] = ImVec4{ buttonActive.r, buttonActive.g, buttonActive.b, buttonActive.a, };
		colours[ImGuiCol_ButtonHovered] = ImVec4{ buttonHovered.r, buttonHovered.g, buttonHovered.b, buttonHovered.a, };

		// Frames
		colours[ImGuiCol_FrameBg] = ImVec4{ frame.r, frame.g, frame.b, frame.a, };
		colours[ImGuiCol_FrameBgActive] = ImVec4{ frameActive.r, frameActive.g, frameActive.b, frameActive.a, };
		colours[ImGuiCol_FrameBgHovered] = ImVec4{ frameHovered.r, frameHovered.g, frameHovered.b, frameHovered.a, };

		// Tabs
		colours[ImGuiCol_Tab] = ImVec4{ tab.r, tab.g, tab.b, tab.a, };
		colours[ImGuiCol_TabActive] = ImVec4{ tabActive.r, tabActive.g, tabActive.b, tabActive.a, };
		colours[ImGuiCol_TabHovered] = ImVec4{ tabHovered.r, tabHovered.g, tabHovered.b, tabHovered.a, };
		colours[ImGuiCol_TabUnfocused] = ImVec4{ tabUnfocused.r, tabUnfocused.g, tabUnfocused.b, tabUnfocused.a, };
		colours[ImGuiCol_TabUnfocusedActive] = ImVec4{ tabUnfocusedHovered.r, tabUnfocusedHovered.g, tabUnfocusedHovered.b, tabUnfocusedHovered.a, };
		colours[ImGuiCol_TabSelectedOverline] = ImVec4{ tabSelectedOverline.r, tabSelectedOverline.g, tabSelectedOverline.b, tabSelectedOverline.a, };

		// Title Bar
		colours[ImGuiCol_TitleBg] = ImVec4{ title.r, title.g, title.b, title.a, };
		colours[ImGuiCol_TitleBgActive] = ImVec4{ titleActive.r, titleActive.g, titleActive.b, titleActive.a, };
		colours[ImGuiCol_TitleBgCollapsed] = ImVec4{ titleCollapsed.r, titleCollapsed.g, titleCollapsed.b, titleCollapsed.a, };
	}

	void ImGuiLayer::OnImGuiRender() {
		static bool show = true;
	}
}