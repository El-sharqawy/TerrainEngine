#include "stdafx.h"
#include "userinterface.h"
#include "../../LibImageUI/imgui.h"
#include "../../LibImageUI/imgui_impl_glfw.h"
#include "../../LibImageUI/imgui_impl_opengl3.h"
#include "../../LibTerrain/source/terrain.h"
#include "../../LibTerrain/source/skybox.h"
#include "../../LibImageUI/ImGuiFileDialog.h"
#include "../../LibImageUI/ImGuiFileDialogConfig.h"
#include "../../LibImageUI/imgui_internal.h"


#if defined(_WIN64)
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

	m_iSelectedBtnIdx = 0;
	m_bShowAboutInfo = false;
	m_bShowDemonWindow = false;
	m_bShowSceneWindow = true;
	m_bShowSkyboxWindow = true;
	m_bInitialized = false;
}

CUserInterface::~CUserInterface()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void CUserInterface::RenderTerrainUI()
{
	bool bEditMode = CScreen::Instance().GetEditingMode();

	if (ImGui::Checkbox("Terrain Editing mode", &bEditMode))
	{
		CScreen::Instance().SetEditingMode(bEditMode);
	}

								// 0		// 1		// 2			// 3			// 4			// 5		
	const char* items[] = { "No Brush", "Up Brush", "Down Brush", "Flatten Brush", "Smooth Brush", "Noise Brush" };
	static int current_item = 0; // Index of the selected item

	if (ImGui::BeginCombo("Terrain Brush", items[current_item])) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < arr_size(items); n++)
		{
			bool is_selected = (current_item == n);
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = n;
				// Update terrain edit mode based on selection

				switch (current_item)
				{
				case 0: // Disable Edit Mode
					CScreen::Instance().SetEditingMode(false);
					CWindow::Instance().SetBrushType(BRUSH_TYPE_NONE);
					break;

				case 1: // Up Brush
					CScreen::Instance().SetEditingMode(true);
					CWindow::Instance().SetBrushType(BRUSH_TYPE_UP);
					break;

				case 2: // Down Brush
					CScreen::Instance().SetEditingMode(true);
					CWindow::Instance().SetBrushType(BRUSH_TYPE_DOWN);
					break;

				case 3: // Flateen Brush
					CScreen::Instance().SetEditingMode(true);
					CWindow::Instance().SetBrushType(BRUSH_TYPE_FLATTEN);
					break;

				case 4: // Smooth Brush
					CScreen::Instance().SetEditingMode(true);
					CWindow::Instance().SetBrushType(BRUSH_TYPE_SMOOTH);
					break;

				case 5: // Noise Brush
					CScreen::Instance().SetEditingMode(true);
					CWindow::Instance().SetBrushType(BRUSH_TYPE_NOISE);
					break;

				default:
					CScreen::Instance().SetEditingMode(false);
					break;
				}

				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
			}
		}
		ImGui::EndCombo();
	}

	GLfloat fBrushRadius = CScreen::Instance().GetBrushRadius();
	GLfloat fBrushStrength = CScreen::Instance().GetBrushStrength();
	if (ImGui::SliderFloat("Brush Radius", &fBrushRadius, 0.0f, 50.0f))
	{
		CScreen::Instance().SetBrushRadius(fBrushRadius);
	}
	if (ImGui::SliderFloat("Brush Strength", &fBrushStrength, 0.0f, 50.0f))
	{
		CScreen::Instance().SetBrushStrength(fBrushStrength);
	}

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();

	static int selectedTextureIndex = -1;
	// ImVec2(height, width)
	ImVec2 buttonSize(125, 25);

	// Create list of texture names for the ListBox
	std::vector<const char*> textureNames;
	for (size_t i = 0; i < CBaseTerrain::Instance().GetTextureSet()->GetTexturesCount(); i++) // Currently Limited to only 4 textures, TODO : make it unlimited !
	{
		if (i == 0)
		{
			textureNames.push_back("Eraser");
			continue;
		}
		const auto& tex = CBaseTerrain::Instance().GetTextureSet()->GetTexture(i);
		if (!tex.m_stFileName.empty())  // Or check tex.m_pTexture != nullptr
		{
			textureNames.push_back(tex.m_stFileName.c_str());
		}
	}

	// ListBox showing loaded textures
	ImGui::Text("Loaded Textures:");
	ImGui::ListBox("##textures_list", &selectedTextureIndex, textureNames.data(), static_cast<GLint>(textureNames.size()), 7);

	// Inside the ListBox display block
	GLint iActualIndex = selectedTextureIndex;
	static int lastSelectedTextureIndex = -2; // -2 to ensure it triggers first time

	ImGui::SameLine();
	if (iActualIndex >= 0 && iActualIndex < CBaseTerrain::Instance().GetTextureSet()->GetTexturesCount())
	{
		if (iActualIndex != lastSelectedTextureIndex)
		{
			auto& selectedTex = CBaseTerrain::Instance().GetTextureSet()->GetTexture(iActualIndex);
			if (selectedTex.m_pTexture && selectedTex.m_pTexture->GetTextureID())
			{
				CWindow::Instance().SetBrushType(BRUSH_TYPE_TEXTURE);
				CBaseTerrain::Instance().GetGeoMipGrid()->SetCurrentTextureIndex(iActualIndex);

				m_iSelectedBtnIdx = 0;

				lastSelectedTextureIndex = iActualIndex;
				sys_err("iActualIndex: %d", iActualIndex);
			}
		}

		// Always show preview image
		auto& selectedTex = CBaseTerrain::Instance().GetTextureSet()->GetTexture(iActualIndex);
		if (selectedTex.m_pTexture && selectedTex.m_pTexture->GetTextureID())
		{
			ImGui::Image((ImTextureID)(intptr_t)selectedTex.m_pTexture->GetTextureID(),
				ImVec2(128, 128), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 0.5));
		}
	}

	ImGui::Spacing();
	if (ImGui::Button("Add Texture", buttonSize))
	{
		IGFD::FileDialogConfig config;
		config.path = ".";
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose Texture", ".png,.jpeg,.tga", config);
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{ // action if OK
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();	// full name
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();		// just the path
			CBaseTerrain::Instance().GetTextureSet()->AddTexture(filePathName, 4.0f, 4.0f, 0.0f, 0.0f, true, 0, 0);
			CBaseTerrain::Instance().DoBindlesslyTexturesSetup();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::SameLine();

	// Remove selected texture button
	if (ImGui::Button("Remove Texture", buttonSize) && iActualIndex > 0)
	{
		if (CBaseTerrain::Instance().GetTextureSet()->RemoveTexture(iActualIndex))
		{
			// Reset selection after removal
			selectedTextureIndex = selectedTextureIndex - 1;
		}
	}

	ImGui::Spacing();
	if (RadioButtonButton("Apply Texture", 0, &m_iSelectedBtnIdx, buttonSize))
	{
		if (iActualIndex >= 0 && iActualIndex < CBaseTerrain::Instance().GetTextureSet()->GetTexturesCount())
		{
			CBaseTerrain::Instance().GetGeoMipGrid()->SetCurrentTextureIndex(iActualIndex);
			CWindow::Instance().SetBrushType(BRUSH_TYPE_TEXTURE);
		}
	}

	ImGui::SameLine();
	if (RadioButtonButton("Eraser", 1, &m_iSelectedBtnIdx, buttonSize))
	{
		CBaseTerrain::Instance().GetGeoMipGrid()->SetCurrentTextureIndex(0);
		selectedTextureIndex = -1;
		CWindow::Instance().SetBrushType(BRUSH_TYPE_ERASER);
	}

	ImGui::Spacing();

	static char filenameBuffer[128] = "resources/terrain/textureset.json";
	ImGui::InputText("##Save As", filenameBuffer, IM_ARRAYSIZE(filenameBuffer));

	if (ImGui::Button("Save Textureset"))
	{
		if (CBaseTerrain::Instance().GetTextureSet()->Save(std::string(filenameBuffer)))
		{
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Saved successfully.");
		}
		else
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to save.");
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Load Textureset", buttonSize))
	{
		IGFD::FileDialogConfig config;
		config.path = ".";
		ImGuiFileDialog::Instance()->OpenDialog("ChooseTexturesetFileDlgKey", "Choose Textureset", ".json", config);
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseTexturesetFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{ // action if OK
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();	// full name
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();		// just the path
			CBaseTerrain::Instance().GetTextureSet()->Load(filePathName);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

void CUserInterface::RenderSceneUI()
{
	SSceneElements& scene = *this->pScene;

	if (ImGui::DragFloat3("Light Position", (float*)&scene.v3LightDir[0], 0.01f, -1.0f, 1.0f))
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
}

void CUserInterface::RenderSkyBoxUI()
{
	CSkyBox::Instance().SetGUI();
}

void CUserInterface::Render()
{
	for (auto obj : m_lObjectsUI)
	{
		if (obj)
		{
			if (obj->bRender)
			{
				obj->SetGUI();
			}
		}
	}

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav;
	ImGui::Begin("Viewport", nullptr, window_flags);
	// Get content region size
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	// Resize framebuffer if needed
	if ((int)viewportPanelSize.x != CWindow::Instance().GetFrameBuffer()->GetWidth() || (int)viewportPanelSize.y != CWindow::Instance().GetFrameBuffer()->GetHeight())
	{
		// CWindow::Instance().GetFrameBuffer()->Resize((int)viewportPanelSize.x, (int)viewportPanelSize.y); // is this really important?
	}
	// Show texture
	ImGui::Image((ImTextureID)(intptr_t)CWindow::Instance().GetFrameBuffer()->GetTextureID(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();


	ImGui::Begin("Terrain Tools");
	if (ImGui::BeginTabBar("##MainEditorTabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Terrain"))
		{
			RenderTerrainUI();
			ImGui::EndTabItem();
		}

		if (m_bShowSceneWindow)
		{
			if (ImGui::BeginTabItem("Scene"))
			{
				RenderSceneUI();
				ImGui::EndTabItem();
			}
		}

		if (m_bShowSkyboxWindow)
		{
			if (ImGui::BeginTabItem("Skybox"))
			{
				RenderSkyBoxUI();
				ImGui::EndTabItem();
			}
		}

		ImGui::EndTabBar();
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

	ImGuiWindowFlags dockspaceFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("MainDockSpace", nullptr, dockspaceFlags);
	ImGui::PopStyleVar(2);

	ImGuiID dockspaceID = ImGui::GetID("MainDockspace");
	ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f),
		ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoSplit);
	ImGui::End();

	// Step 2: Only once: create layout
	if (!m_bInitialized)
	{
		ImGui::DockBuilderRemoveNode(dockspaceID);
		ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

		ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.25f, nullptr, &dockspaceID);
		ImGuiID dock_center_id = dockspaceID;

		ImGui::DockBuilderDockWindow("Viewport", dock_center_id);
		ImGui::DockBuilderDockWindow("Terrain Tools", dock_left_id);

		ImGui::DockBuilderFinish(dockspaceID);
		m_bInitialized = true;
	}

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
				CBaseTerrain::Instance().bRender = !CBaseTerrain::Instance().bRender;
			}

			if (ImGui::MenuItem("Scene"))
			{
				m_bShowSceneWindow = !m_bShowSceneWindow;
			}

			if (ImGui::MenuItem("SkyBox"))
			{
				CSkyBox::Instance().bRender = !CSkyBox::Instance().bRender;
			}

			if (ImGui::MenuItem("Demo"))
			{
				m_bShowDemonWindow = !m_bShowDemonWindow;
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

	for (auto& it : m_lObjectsUI)
	{
		it->Update();
	}
}

void CUserInterface::AddObject(CObject* pObject)
{
	m_lObjectsUI.push_back(pObject);
}