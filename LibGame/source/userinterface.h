#pragma once

#include "../../LibGL/source/window.h"
#include "../../LibTerrain/source/object.h"

#include <list>
#include <unordered_map>

class CUserInterface : public CObject
{
public:
	CUserInterface(CWindow* pWindow);
	~CUserInterface();

	virtual void Render();
	virtual void Update();

	void AddObject(CObject * pObject);

	void RenderTerrainUI();
	void RenderSceneUI();
	void RenderSkyBoxUI();

private:
	CWindow* m_pWindow;
	std::list<CObject*> m_lObjectsUI;

	bool m_bShowDemonWindow;
	bool m_bShowAboutInfo;
	bool m_bShowSceneWindow;
	bool m_bShowSkyboxWindow;

	bool m_bInitialized = false;

	GLint m_iSelectedBtnIdx;

public:
	bool ToggleButton(const char* label, bool* p_state, const ImVec2& size = ImVec2(0, 0),
		const ImVec4& pressedColor = ImVec4(0.2f, 0.2f, 0.8f, 1.0f),
		const ImVec4& hoveredColor = ImVec4(0.3f, 0.3f, 0.9f, 1.0f),
		const ImVec4& activeColor = ImVec4(0.2f, 0.2f, 0.8f, 1.0f)
	)
	{
		bool clicked = *p_state;

		if (clicked)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, pressedColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
		}

		if (ImGui::Button(label, size))
		{
			*p_state = !*p_state;
		}

		if (clicked)
		{
			ImGui::PopStyleColor(3);
		}

		return clicked;
	}

	bool RadioButtonButton(const char* label, int index, int* selectedIndex,
		const ImVec2& size = ImVec2(0, 0),
		const ImVec4& selectedColor = ImVec4(0.2f, 0.2f, 0.8f, 1.0f),
		const ImVec4& hoveredColor = ImVec4(0.3f, 0.3f, 0.9f, 1.0f),
		const ImVec4& activeColor = ImVec4(0.2f, 0.2f, 0.8f, 1.0f))
	{
		bool isSelected = (*selectedIndex == index);

		if (isSelected)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, selectedColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
		}

		bool clicked = ImGui::Button(label, size);

		if (isSelected)
			ImGui::PopStyleColor(3);

		if (clicked)
			*selectedIndex = index;

		return clicked;
	}

};