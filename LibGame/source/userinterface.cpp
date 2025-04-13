#include "stdafx.h"
#include "userinterface.h"
#include "../../LibImageUI/imgui.h"
#include "../../LibImageUI/imgui_impl_glfw.h"
#include "../../LibImageUI/imgui_impl_opengl3.h"
#include "../../LibTerrain/source/midpoint_terrain.h"

#if defined(_WIN64)
#include <shellapi.h>

#undef min
#undef max
#undef minmax
#endif

CUserInterface::CUserInterface(CWindow* pWindow)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark(); // Could be Light Too

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(pWindow->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	m_pWindow = pWindow;
	m_bShowAboutInfo = false;
	m_bShowDemonWindow = false;
}

CUserInterface::~CUserInterface()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void CUserInterface::Render()
{
	SSceneElements& scene = *this->pScene;
	for (auto obj : m_lObjectsUI)
	{
		if (obj)
		{
			obj->SetGUI();
		}
	}

	ImGui::Begin("Scene controls: ");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Other controls");
	if (ImGui::DragFloat3("Light Position", (float*)& scene.v3LightDir[0], 0.01f, -1.0f, 1.0f))
	{
		auto saturate = [](float v) { return std::min(std::max(v, 0.0f), 0.8f); };
		scene.v3LightDir.y = saturate(scene.v3LightDir.y);
	}
	ImGui::InputFloat3("Camera Position", (float*)&scene.pCamera->GetPosition()[0]);
	ImGui::ColorEdit3("Light color", (float*)&scene.v3LightColor);
	ImGui::ColorEdit3("Fog color", (float*)&scene.v3FogColor);

	float camSpeed = scene.pCamera->GetSpeed();
	float camMaxSpeed = scene.pCamera->GetMaxSpeed();

	if (ImGui::SliderFloat("Camera speed", &camSpeed, 0.0f, camMaxSpeed))
	{
		scene.pCamera->SetSpeed(camSpeed);
	}

	ImGui::Checkbox("Wireframe mode", &scene.bWireFrame);

	if (ImGui::Button("Generate seed"))
	{
		scene.v3Seed = MyMath::GenerateRandomVector3();
	}
	ImGui::SameLine();
	if (ImGui::Button("Use default seed"))
	{
		scene.v3Seed = SVector3Df(0.0, 0.0, 0.0);
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	//actual drawing
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

}

void CUserInterface::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (m_bShowDemonWindow)
		ImGui::ShowDemoWindow(&m_bShowDemonWindow);

	ImGui::GetStyle().FramePadding = ImVec2(8, 6); // Padding for menu items
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Dark background

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				/* Action */
			}

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				/* Action */
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				glfwSetWindowShouldClose(m_pWindow->GetWindow(), true);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z"))
			{
				/* Action */
			}
			if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
			{
				// Disabled
			} 
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Terrain"))
			{
				/* Action */
			}

			if (ImGui::MenuItem("SkyBox"))
			{
				/* Action */
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation", "F1"))
			{
				// Open documentation URL
#if defined(_WIN64)
				//ShellExecute(0, 0, L"http://www.google.com", 0, 0, SW_SHOW); // WINDOWS ONLY
				system("start https://github.com/el-sharqawy");
#else
				//system("mozilla http://google.com");
				system("xdg-open https://github.com/el-sharqawy");
#endif
			}

			if (ImGui::MenuItem("Check for Updates"))
			{
				// Implement update check logic
			}

			ImGui::Separator();

			if (ImGui::MenuItem("About"))
			{
				m_bShowAboutInfo = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleColor();

	// About Window
	if (m_bShowAboutInfo)
	{
		ImGui::OpenPopup("About");
		m_bShowAboutInfo = false;
	}

	// Always center About window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "My Awesome Application");
		ImGui::Separator();

		// Version info
		ImGui::Text("Version: %s", "1.0.0");
		ImGui::Text("Build Date: %s %s", __DATE__, __TIME__);

		// Credits
		ImGui::Spacing();
		ImGui::Text("Developed by:");
		ImGui::BulletText("Osama Elsharqawy");
		ImGui::BulletText("elsharqawy2@gmail.com");

		// GitHub link
		ImGui::Spacing();
		if (ImGui::Button("GitHub Repository"))
		{
#if defined(_WIN64)
			//ShellExecute(0, 0, L"http://www.google.com", 0, 0, SW_SHOW); // WINDOWS ONLY
			system("start https://github.com/el-sharqawy");
#else
			//system("mozilla http://google.com");
			system("xdg-open https://github.com/el-sharqawy");
#endif
			ImGui::CloseCurrentPopup();
		}

		// License information
		ImGui::Spacing();
		ImGui::Text("License: MIT");
		ImGui::TextWrapped("This software is provided 'as-is', without any express\n"
			"or implied warranty. See LICENSE for details.");

		// Close button
		ImGui::Separator();
		float button_width = 120.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - button_width) * 0.5f);
		if (ImGui::Button("Close", ImVec2(button_width, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void CUserInterface::AddObject(CObject* pObject)
{
	m_lObjectsUI.push_back(pObject);
}